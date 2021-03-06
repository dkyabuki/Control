#include "actuator.h"

int A_Init()
{
	channel = 0;
    return 0;
}

int A_Calibration()
{
	DSCDACALPARAMS dscdacalparams;
	memset(&dscdacalparams, 0, sizeof(DSCDACALPARAMS));
	dscdacalparams.darange = 65535;

	if( ( dscDAAutoCal( dscb, &dscdacalparams ) ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		rt_printf("dscDAAutoCal error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return 0;
	}

	if( ( dscDACalVerify( dscb, &dscdacalparams ) ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		rt_printf("dscDACalVerify error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return 0;
	}

	printf( "Offset Error: %f, Gain Error: %f\n", dscdacalparams.offset, dscdacalparams.gain );

	if ( fabs( dscdacalparams.offset ) > 2.0 ||
		 fabs( dscdacalparams.gain ) > 2.0 )
		rt_printf( "Value for offset or gain exceeded specified tolerance\n" );
	else rt_printf( "Values for offset and gain met specified tolerance\n" );

	rt_printf("Calibração dos conversores DA... COMPLETA\n\n" );
	return 0;
}

int A_Kill()
{
	dscFree();
	return 0;
}

int A_Core()
{

    // send DACODE of 4095 - Full scale to channel 0.
//    dscDAConvert(dscb, channel , (int)4095*((setpoint-2.2466)/0.6380));
    dscDAConvert(dscb, channel , (int)4095*((setpoint+20)/40.0));
	return 0;
}

void send_setpoint(double value)
{
	dscDAConvert(dscb, channel , (int)4095*((value+20)/40));
	sleep(2);
}
