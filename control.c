#include "control.h"
#include "global.h"

int C_Init()
{
	POSITION[0] = 0;
	POSITION[1] = 0;
	setpoint = 0;

//	3.999464053756402e-04, 3.998930827290227e-04, -1.999592081594453, 0.999600079989334
	/********** INSTAVEL ***********/
	/*a = 3.999464053756402e-04;
	b = 3.998930827290227e-04;
	c = 1;
	d = -1.999592081594453;
	e = 0.999600079989334;*/
	/********** ESTAVEL ***********/
//	a = 0.096093397200609;
//	b = 0.093564670856204;
//	c = 1;
//	d = -1.920587572145879;
//	e = 0.923116346386636;

	Tsample = 0.002;
	J = 0.000001*0.112;
	B = 0.000001*2.5365;
	K = 0.000001*8.6590;

//	K = 1;
//	B = pow(cos(beta), 2)*K*Ts/2;
//	J = pow(cos(beta), 2)*K*pow(Ts, 2)/16;

////parametros exo livre
//	a2 = -8319.76969;
//	a1 = 5905.69681;
//	a0 = 2414.10338;
//	b2 = 1;
//	b1 = 2;
//	b0 = 1;

//parametros conjunto
//	a1 = 1;
//	a2 = -0.5094;
//	a3 = -0.4891;
//	b1 = 0.00025084;
//	b2 = -0.00063709;
//	b3 = -0.00038761;

	if(DISCRETIZATION == TUSTIN)
	{
		a = 1;
		b = 2;
		c = 1;
		d = 4*J/pow(Tsample, 2) + 2*B/Tsample + K;
		e = -8*J/pow(Tsample, 2) + 2*K;
		f = 4*J/pow(Tsample, 2) - 2*B/Tsample + K;
	}
	else if(DISCRETIZATION == BACKWARD_DIFF)
	{
		a = 1;
		d = J/pow(Tsample, 2) + B/Tsample + K;
		e = -2*J/pow(Tsample, 2) -B/Tsample;
		f = J/pow(Tsample, 2);
	}
	trajectory_time = 0;
	return 0;
}

int C_Kill()
{
	return 0;
}

int C_Core()
{
	if(TRAJECTORY == SINUSOIDAL_POS){			//senoidal
		trajectory_time += 2;
//		SETPOINT[2] = 2.5656 + 0.3190*sin(2*M_PI*trajectory_time/TRAJ_PERIOD);
		setpoint = 20*sin(2*M_PI*trajectory_time/TRAJ_PERIOD);
	}
	else if(TRAJECTORY == CONSTANT)				//cte
	{
		setpoint = SET_CTE;
	}
	else if(TRAJECTORY == SINUSOIDAL_TOR || TRAJECTORY == CONTROL_ACTIVE)				//controle ativado
	{
//		POSITION[0] = POSITION[1];
//		POSITION[1] = POSITION[2];

	//	position = a*TORQUE[1]+b*TORQUE[0]-d*SETPOINT[1]-e*SETPOINT[0];
	//	position = position/c;


		if(DISCRETIZATION == TUSTIN)
		{
			setpoint = c*TORQUE[2]+b*TORQUE[1]+a*TORQUE[0]-e*POSITION[1]-f*POSITION[0];
			setpoint = setpoint/d;
		}
		else if (DISCRETIZATION == BACKWARD_DIFF)
		{
			setpoint = a*TORQUE[0]-e*POSITION[1]-f*POSITION[0];
			setpoint = setpoint/d;
		}

		if (setpoint > 20) setpoint = 20;
		if (setpoint < -20) setpoint = -20;

	//	if (SETPOINT[2] > 2.8846) SETPOINT[2] = 2.8846;
	//	if (SETPOINT[2] < 2.2466) SETPOINT[2] = 2.2466;

		//DADOS PARAMETROS A1-3 E B1-3
	//	position = b2*TORQUE[2]+b1*TORQUE[1]+b0*TORQUE[0]-a1*SETPOINT[1]-a0*SETPOINT[0];
	//	position = position/a2;
	//
	//	SETPOINT[2] = position;
	//
	//    if (SETPOINT[2] > 2.8846) SETPOINT[2] = 2.8846;
	//    if (SETPOINT[2] < 2.2466) SETPOINT[2] = 2.2466;


	}
	return 0;
}


