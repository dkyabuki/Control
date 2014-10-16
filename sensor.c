#include "sensor.h"


int S_Init()
{
	//inicializar variaveis globais do sensor
	osc_time = 0;
	TORQUE[0] = 0;
	TORQUE[1] = 0;
	TORQUE[2] = 0;
	k=0;
	readingOffset = 0;
	file = fopen("teste_hum_2_1210.txt","w");

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

//void set_reference_voltage ()
//{
//	double temp;
//	if( ( result = dscADScan( dscb, &dscadscan, samples ) ) != DE_NONE )
//	{
//		dscGetLastError(&errorParams);
//		fprintf( stderr, "dscADScan error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
//		return;
//	}
//
//	if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[0], &temp) != DE_NONE)
//	{
//		dscGetLastError(&errorParams);
//		fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
//		return;
//	}
//
////	voltagereference = (temp+0.3273)/(-0.2093);
//}
double S_CalcOffset()
{
	int i;
	double reading_ext;
	double sum = 0;
	for (i = 0; i<10000; i++)
	{
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
		sum += reading_ext;
		usleep(100);
	}
	readingOffset = sum/10000.0 - 3.24;
	return readingOffset;
}


double read_potentiometer()
{
	double reading_pot, reading_vcc;
	if( ( result = dscADScan( dscb, &dscadscan, samples ) ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		fprintf( stderr, "dscADScan error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return -1;
	}

	if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[2], &reading_pot) != DE_NONE)
	{
		dscGetLastError(&errorParams);
		fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return -1;
	}

	if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[1], &reading_vcc) != DE_NONE)
	{
		dscGetLastError(&errorParams);
		fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return -1;
	}
	return ((reading_vcc-reading_pot)*5.00/reading_vcc);
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
	dscFree();
	return 0;
}

int S_Core()
{
//	clock_gettime(CLOCK_REALTIME, &start);
	DFLOAT potentiometer_reading;
	DFLOAT voltage;
	int i;
//	double fc = 5;
	double torque;
//	double deadzone = 0.04;

	if( ( result = dscADScan( dscb, &dscadscan, samples ) ) != DE_NONE )
		{
		dscGetLastError(&errorParams);
		fprintf( stderr, "dscADScan error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		free( samples ); // remember to deallocate malloc() memory
		return 0;
	}

	//if (SENSORING) rt_printf( "\nActual voltages:" );

	for( i = 0; i < (dscadscan.high_channel - dscadscan.low_channel)+ 1; i++)
	{
		if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[i], &voltage) != DE_NONE)
		{
			dscGetLastError(&errorParams);
			fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
			free(samples);
			return 0;
		}

		if (SENSORING) rt_printf(" %5.3lfV, i=%d", voltage, i);
		if (i==0)
		{
			//DEADZONE = .04;


			TORQUE[0] = TORQUE[1];
			TORQUE[1] = TORQUE[2];
			torque = voltage;

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
			*/
//			if(voltage<=-deadzone || voltage>=deadzone){
//				torque = voltage;
//				TORQUE[2]=(torque-readingOffset-3.2368)/(-0.0865);
//			} else {
//				TORQUE[2] = 0;//torque;
//			}


			if (TRAJECTORY == 3){
				osc_time += 2;
				TORQUE[2]=0.1*sin(2*M_PI*osc_time/OSC_PERIOD);
			}else{
//				TORQUE[2]=torque;
				TORQUE[2]=(torque-readingOffset-3.2368)/(-0.0865);
//				TORQUE[2]=(torque+0.3273)/(-0.2093);
//				TORQUE[2]=(torque+0.0058)/(-0.0693);
			}

//					TORQUE[2] = TORQUE[1]*(1-0.002*2*M_PI*fc) + torque*(0.002*2*M_PI*fc);

			if (SENSORING) printf("\n Torque: %5.3lfNm %5.3lfNm %5.3lfNm", TORQUE[0], TORQUE[1], TORQUE[2]);

		} else if (i == 1) {
			double voltagepot;
			if( dscADCodeToVoltage(dscb, dscadsettings, dscadscan.sample_values[i+1], &voltagepot) != DE_NONE)
			{
				dscGetLastError(&errorParams);
				fprintf( stderr, "dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
				free(samples);
				return 0;
			}

			//potenciometro_atual = (voltage-voltagepot)*5.0/voltage;
			potentiometer_reading = 62.9774*voltagepot - 164.6252;
			rt_printf("\t%5.8f \t%5.8f \t", torque-readingOffset, potentiometer_reading);
			if(k >= 0 && k < 16000){ 	//salva
//			if(k > 2 && k <= 1002){
				saveToFile (file, torque);
				rt_printf("\t%d\n", k);
				k++;
			} else if(k == 16000){ 		//para
//			} else if(k == 1003){
				rt_printf("\t%d\n", k);
				closeFile(file);
				k++;
//			} else if(k <= 0){ 			//wait
//				rt_printf("\t%d \tWAITING!\n", k);
//				k++;
			} else { 					//finish
				rt_printf("\tDONE!\n");
			}
			POSITION[0] = POSITION[1];
			POSITION[1] = POSITION[2];
			POSITION[2] = potentiometer_reading;
		}


	}
//	clock_gettime(CLOCK_REALTIME, &end);
//	double timespent = ( end.tv_sec-start.tv_sec ) + ( end.tv_nsec-start.tv_nsec )/BILLION;
//	rt_printf("\t%5.8f \n", timespent*1000.00);
	return 0;
}

void saveToFile (FILE *f, double tor){
	if (file != NULL){
		fprintf(f, "%5.8f, %5.8f, %5.8f, %5.8f,\n", TORQUE[2], POSITION[2], setpoint, tor);
	}
}

void closeFile (FILE *f){
	if (file != NULL){
		fclose(f);
	}
}
