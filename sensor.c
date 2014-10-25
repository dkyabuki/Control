#include "sensor.h"


int S_Init()
{
	//inicializar variaveis globais do sensor
	osc_time = 0;
	TORQUE[0] = 0;
	TORQUE[1] = 0;
	TORQUE[2] = 0;
	POSITION[0] = 0;
	POSITION[1] = 0;
	POSITION[2] = 0;
	extensometerOffset = 0;
	potentiometerOffset = 0;

	//inicializa as configuracoes do conversor AD
	memset(&dscadsettings, 0, sizeof(DSCADSETTINGS)); //zera valores de dscadsettings
	samples = (DSCSAMPLE*)malloc( sizeof(DSCSAMPLE) * ( dscadscan.high_channel - dscadscan.low_channel + 1 ) );

	dscadsettings.range = RANGE_5;
	dscadsettings.gain = GAIN_1;
	dscadsettings.load_cal = (BYTE)FALSE;
	dscadsettings.current_channel = 0;

	dscadscan.low_channel = 0;
	dscadscan.high_channel = 2;
	dscadscan.gain = dscadsettings.gain;

	if( ( result = dscADSetSettings( dscb, &dscadsettings ) ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		rt_printf( "dscADSetSettings error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return 0;
	}

	return 0;
}

void S_CalcOffset()
{
	int i;
	double reading_ext, reading_pot;
	double sumtor = 0;
	double sumpos = 0;
	for (i = 0; i<10000; i++)
	{
		if( ( result = dscADScan( dscb, &dscadscan, samples ) ) != DE_NONE )
		{
			dscGetLastError(&errorParams);
			fprintf( stderr, "dscADScan error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
			return;
		}
		if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[0], &reading_ext) != DE_NONE)
		{
			dscGetLastError(&errorParams);
			fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
			return;
		}
		sumtor += reading_ext;
		if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[2], &reading_pot) != DE_NONE)
		{
			dscGetLastError(&errorParams);
			fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
			return;
		}
		sumpos += reading_pot;
		usleep(100);
	}
	extensometerOffset = sumtor/10000.0 - 3.198; //3.24
	potentiometerOffset = 62.9774*sumpos/10000.0;
}

double S_getTorque()
{
	double reading_ext;
	if( ( result = dscADScan( dscb, &dscadscan, samples ) ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		fprintf( stderr, "dscADScan error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return -1;
	}
	if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[0], &reading_ext) != DE_NONE)
	{
		dscGetLastError(&errorParams);
		fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return -1;
	}
	return (reading_ext);
}

int S_Calibration(){
	int i;

	rt_printf( "Calibração dos conversores AD...\n\n" );

	dscadcalparams.adrange = 0xFF;
	dscadcalparams.boot_adrange = 0;

	dscautocal.adrange = 0xFF;
	dscautocal.boot_adrange = 0;

	if( (result = dscADAutoCal( dscb, &dscautocal )) != DE_NONE )
 	{
		dscGetLastError(&errorParams);
		rt_printf("dscADAutoCal error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
 		return 0;
 	}

	for( i = 0; i < 16; i++ )
	{
		if ( ( i > 3 && i < 8 )  )
			continue;

		dscautocal.adrange = i;
		dscautocal.ad_gain = 0;
		dscautocal.ad_offset = 0;

		if( ( result = dscADCalVerify( dscb, &dscautocal ) ) != DE_NONE )
		{
			dscGetLastError(&errorParams);
			rt_printf( "dscADCalVerify error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
			return 0;
		}

		rt_printf( "Configuration Mode: %d, Offset Error: %9.3f, Gain Error: %9.3f\n", i, dscautocal.ad_offset, dscautocal.ad_gain );

		if ( fabs( dscautocal.ad_offset ) > 2.0f ||
			 fabs( dscautocal.ad_gain ) > 2.0f )
			rt_printf( "Value for offset or gain exceeded specified tolerance\n" );
		else rt_printf( "Values for offset and gain met specified tolerance\n" );
	}

	rt_printf( "Calibração dos conversores AD... COMPLETA\n\n" );
	return 0;
}

int S_Kill()
{
	free(samples);
	return 0;
}

int S_Core()
{
//	clock_gettime(CLOCK_REALTIME, &start);
//	double deadzone = 0.2;

	if( ( result = dscADScan( dscb, &dscadscan, samples ) ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		fprintf( stderr, "dscADScan error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		free( samples );
		return 0;
	}

	int i;
	for( i = 0; i < (dscadscan.high_channel - dscadscan.low_channel)+ 1; i++)
	{
		if (i == 0)				//TORQUE
		{
			if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[i], &torque_reading) != DE_NONE)
			{
				dscGetLastError(&errorParams);
				fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
				return 0;
			}
			/*
			//DEADZONE
			torque = (voltage-voltagereference);
			if 		(TORQUE[1]-torque<=-deadzone)
				TORQUE[2]=torque-deadzone;
			else if (TORQUE[1]-torque>=deadzone)
				TORQUE[2]=torque;
			else TORQUE[2]=TORQUE[1]+deadzone;

					torque = (voltage-voltagereference);
					if 		(TORQUE[1]-torque<=-deadzone)
						TORQUE[2]=torque;
					else if (TORQUE[1]-torque>=deadzone)
						TORQUE[2]=torque;
					else TORQUE[2]=TORQUE[1];

//			if(voltage<=readingOffset-deadzone || voltage>=readingOffset+deadzone){
//				torque = voltage;
//				TORQUE[2]=(torque-readingOffset-3.198)/(-0.0865);
//			} else {
//				TORQUE[2] = 0;//torque;
//			}


//			if (TRAJECTORY == 3){
//				osc_time += 2;
//				TORQUE[2]=0.1*sin(2*M_PI*osc_time/OSC_PERIOD);
//			}else{
//				TORQUE[2]=(torque+0.3273)/(-0.2093);
//				TORQUE[2]=(torque-extensometerOffset-3.198)/(-0.0865);
//				TORQUE[2]=(torque+0.0058)/(-0.0693);
//			}
			*/

		} else if (i == 1) 		//POSITION
		{
			if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[i+1], &position_reading) != DE_NONE)
			{
				dscGetLastError(&errorParams);
				fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
				free(samples);
				return 0;
			}
		} else
			break;
	}
//	clock_gettime(CLOCK_REALTIME, &end);
//	double timespent = ( end.tv_sec-start.tv_sec ) + ( end.tv_nsec-start.tv_nsec )/BILLION;
//	rt_printf("\t%5.8f \n", timespent*1000.00);
	return 0;
}
