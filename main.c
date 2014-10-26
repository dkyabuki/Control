#include "tasks.h"
#include "include.h"
#include "global.h"

//Declarações
void initBoard();
void menu();
void config();
void task_config();
void calibration();
void set_task_priority();
void set_task_period();
void enable_task();
int waitForUser ();
//void set_zero_position();

int main(int argc, char* argv[])
{
	initBoard();
	MONITORING = 0;
	SENSORING = 0;
	TRAJECTORY = CONTROL_ACTIVE;	//1-SINUSOIDAL_POS 2-CONSTANT 3-SINUSOIDAL_TOR 4-CONTROL_ACTIVE
	DISCRETIZATION = BACKWARD_DIFF;

//	printf("Digite os parâmetros J, B e K, nesta ordem\n");
//	scanf ("%lf", &J);
//	scanf ("%lf", &B);
//	scanf ("%lf", &K);
	T_Xenomai();
	T_Init();
	T_Create();

	send_setpoint(SET_CTE);
	printf("Calculando offsets...\n");
	sleep(5);
	S_CalcOffset();
	printf("Offsets calculados: \n  Torque: %5.8f \t Posição: %5.8f \n", potentiometerOffset, extensometerOffset);
	printf("Iniciando a comunicação\n");
//	Start_Comm();

//	readingOffset = -0.098;

//	if(TRAJECTORY == SINUSOIDAL_POS){
//		send_setpoint(SET_CTE);
//	}

//	else
//	{
//		send_setpoint(2.5656);
//	}
//	set_reference_voltage();

	menu();
	T_Start();
	sleep(1);
	waitForUser();
	waitForUser();
	startSaving();
	waitForUser();
	finishSaving();

	pause();
	T_Kill();
	return 0;
}

void initBoard()
{
	dsccb.boardtype = DSC_DMM16AT;
	dsccb.base_address=0x300;		//endereço de base da placa
	dsccb.int_level = 7;			//nível de interrupção
	dsccb.clock_freq = 10000000;

	//Inicializa a biblioteca dscud
	if( dscInit( DSC_VERSION ) != DE_NONE )
	{
		dscGetLastError(&errorParams);
		rt_printf("dscInit error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return;
	}

	//Inicializa a Placa
	if(dscInitBoard(DSC_DMM16AT, &dsccb, &dscb)!= DE_NONE)
	{
		dscGetLastError(&errorParams);
		rt_printf("dscInitBoard error: %s %s\n", dscGetErrorString(errorParams.ErrCode), errorParams.errstring );
		return;
	}
}

void menu()
{
	int choice = 0;
	while (1)
	{
		printf("Controle de Impedâncias\n");
		printf("1. Configurações\n");
		printf("2. Executar\n\n");

		scanf ("%d",&choice);

		if(choice == 1)
			config();

		else if(choice == 2)
			break;

		else
			printf("Opção inválida!\n\n");
	}
}

void config()
{
	int choice = 0;
	while (1)
	{
		printf("Configurações\n");
		printf("1. Configurar Tasks\n");
		printf("2. Calibrar conversores\n");
		printf("3. MONITORING = %hd\n",MONITORING);
		printf("4. SENSORING = %hd\n",SENSORING);
		printf("5. TRAJECTORY = %hd\n", TRAJECTORY);
		printf("6. Voltar\n\n");
		scanf ("%d",&choice);
		if(choice == 1) task_config();

		else if(choice == 2)
			calibration();

		else if(choice == 3)
			scanf ("%hd",&MONITORING);

		else if(choice == 4)
			scanf ("%hd",&SENSORING);

		else if(choice == 5)
			scanf ("%hd",&TRAJECTORY);

		else if(choice == 6)
			break;

		else
			printf("Opção inválida!\n\n");
	}
}

void task_config()
{
	int choice = 0;
	while (1)
	{
		printf("Configurações das Tasks\n");
		printf("1. Habilitar/Desabilitar Task\n");
		printf("2. Configurar período das Tasks [ms]\n");
		printf("3. Configurar prioridade das Tasks [0-100]\n");
		printf("4. Voltar\n\n");
		scanf ("%d",&choice);

		if(choice == 1)
			enable_task();

		else if(choice == 2)
			set_task_period();

		else if(choice == 3)
			set_task_priority();

		else if(choice == 4)
			break;

		else
			printf("Opção inválida!\n\n");
	}
}

void enable_task()
{
	int choice = 0;
	while (1)
	{
		printf("Habilita/Desabilita Task\n");
		printf("1. ENABLETASK[SENSOR]   =  %d\n",ENABLETASK[SENSOR]);
		printf("2. ENABLETASK[CONTROL]  =  %d\n",ENABLETASK[CONTROL]);
		printf("3. ENABLETASK[ACTUATOR] =  %d\n",ENABLETASK[ACTUATOR]);
		printf("4. ENABLETASK[TEST]     =  %d\n",ENABLETASK[TEST]);
		printf("5. Voltar\n\n");
		scanf ("%d",&choice);

		if(choice == 1)
			scanf ("%d",&ENABLETASK[SENSOR]);

		else if(choice == 2)
			scanf ("%d",&ENABLETASK[CONTROL]);

		else if(choice == 3)
			scanf ("%d",&ENABLETASK[ACTUATOR]);

		else if(choice == 4)
			scanf ("%d",&ENABLETASK[TEST]);

		else if(choice == 5)
			break;

		else
			printf("Opção inválida!\n\n");
	}
}

void set_task_period()
{
	int choice = 0;
	while (1)
	{
		printf("Período das Tasks [ms]\n");
		printf("1. PERIOD[SENSOR]   =  %d\n",PERIOD[SENSOR]);
		printf("2. PERIOD[CONTROL]  =  %d\n",PERIOD[CONTROL]);
		printf("3. PERIOD[ACTUATOR] =  %d\n",PERIOD[ACTUATOR]);
		printf("4. PERIOD[TEST]     =  %d\n",PERIOD[TEST]);
		printf("5. Voltar\n\n");
		scanf ("%d",&choice);

		if(choice == 1)
			scanf ("%d",&PERIOD[SENSOR]);

		else if(choice == 2)
			scanf ("%d",&PERIOD[CONTROL]);

		else if(choice == 3)
			scanf ("%d",&PERIOD[ACTUATOR]);

		else if(choice == 4)
			scanf ("%d",&PERIOD[TEST]);

		else if(choice == 5)
			break;

		else
			printf("Opção inválida!\n\n");
	}
}

void set_task_priority()
{
	int choice = 0;
	while (1)
	{
		printf("Prioridade das Tasks [0-100]\n");
		printf("1. Prioridade do Sensor    =  %d\n",PRIORITY[SENSOR]);
		printf("2. Prioridade do Controle  =  %d\n",PRIORITY[CONTROL]);
	    printf("3. Prioridade do Atuador   =  %d\n",PRIORITY[ACTUATOR]);
		printf("4. Prioridade do Teste     =  %d\n",PRIORITY[TEST]);
		printf("5. Voltar\n\n");
		scanf ("%d",&choice);

		if(choice == 1)
			scanf ("%d",&PRIORITY[SENSOR]);

		else if(choice == 2)
			scanf ("%d",&PRIORITY[CONTROL]);

		else if(choice == 3)
			scanf ("%d",&PRIORITY[ACTUATOR]);

		else if(choice == 4)
			scanf ("%d",&PRIORITY[TEST]);

		else if(choice == 5)
			break;

		else
			printf("Opção inválida!\n\n");
	}
}

void calibration()
{
	int choice = 0;
	while (1)
	{
		printf("Calibração dos conversores\n");
		printf("1. Conversores Digital-Analógicos\n");
		printf("2. Conversores Analógico-Digitais\n");
		printf("3. Voltar\n\n");
		scanf ("%d",&choice);

		if(choice==1)
			A_Calibration();

		else if(choice==2)
			S_Calibration();

		else if(choice==3)
			break;

		else
			printf("Opção inválida!\n\n");
	}
}

//void set_zero_position()
//{
//	double stack, ideal, difference, tolerance, setpoint, variation;
//	variation = 10;
////	ideal = 3.3459;
//	ideal = 3.15;
//	tolerance = 0.005;
//	setpoint = 0;
//	int counter;
//	while(1)
//	{
//		send_setpoint(setpoint);
//		stack = 0.0;
//		for(counter = 0; counter < 100; counter++){
//			stack += read_potentiometer();
//		}
//		stack = stack/counter;
//		difference = ideal - stack;
//		if (fabs(difference) <= tolerance)
//			break;
//		else if (difference < 0)
//			setpoint += variation;
//		else
//			setpoint -= variation;
//		variation /= 2;
//		printf("%lf\t %lf\n", stack, difference);
//	}
//	printf("%lf\n", setpoint);
//	setpoint_offset = setpoint;
//}

int waitForUser ()
{
  int ch;
  struct termios oldt, newt;

  tcgetattr ( STDIN_FILENO, &oldt );
  newt = oldt;
  newt.c_lflag &= ~( ICANON | ECHO );
  tcsetattr ( STDIN_FILENO, TCSANOW, &newt );
  ch = getchar();
  tcsetattr ( STDIN_FILENO, TCSANOW, &oldt );

  return ch;
}
