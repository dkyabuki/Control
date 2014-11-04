#include "tasks.h"

void T_Init()
{
	//Declaração do ponteiro de funções
	pfunction[SENSOR]    = S_Core;
	pfunction[CONTROL]   = C_Core;
	pfunction[ACTUATOR]  = A_Core;
	pfunction[COMM]  	 = Comm_Core;
	pfunction[TEST]      = T_Test;

	//Habilita/Desabilita Task
	ENABLETASK[SENSOR]   = FALSE;
	ENABLETASK[CONTROL]  = FALSE;
	ENABLETASK[ACTUATOR] = FALSE;
	ENABLETASK[COMM] 	 = TRUE;
	ENABLETASK[TEST] 	 = FALSE;

	//Período das Tasks [ms]
	PERIOD[SENSOR]   = 2;
	PERIOD[CONTROL]  = 2;
	PERIOD[ACTUATOR] = 2;
	PERIOD[COMM] 	 = 2;
	PERIOD[TEST] 	 = 2;

	//Prioridade das Tasks [0-100]
	PRIORITY[SENSOR]   = 50;
	PRIORITY[CONTROL]  = 50;
	PRIORITY[ACTUATOR] = 50;
	PRIORITY[COMM] 	   = 50;
	PRIORITY[TEST] 	   = 50;

	A_Init();
	C_Init();
	S_Init();
	Comm_Init();
}

void T_Kill()
{
//	A_Kill();
//	C_Kill();
//	S_Kill();
	dscFree();
}

void T_Xenomai()
{
	//	 Avoids memory swapping for this program
	mlockall(MCL_CURRENT|MCL_FUTURE);
	//	 Perform auto-init of rt_print buffers if the task doesn't do so
	rt_print_auto_init(1);
	rt_timer_set_mode(1000000); //1ms -not working for periodic mode
}

void T_Create()
{
	rt_task_create(&TaskSensor, "Sensor", 0, PRIORITY[SENSOR], 0);
	rt_task_create(&TaskControl, "Controle", 0, PRIORITY[CONTROL], 0);
	rt_task_create(&TaskActuator, "Atuador", 0, PRIORITY[ACTUATOR], 0);
	rt_task_create(&TaskComm, "Comunicação", 0, PRIORITY[COMM], 0);
	rt_task_create(&TaskTest, "Teste", 0, PRIORITY[TEST], 0);
}

void T_Start()
{
	if (ENABLETASK[SENSOR]) {
//		S_Init();
		rt_task_start(&TaskSensor, &T_Monitor, SENSOR);
	}
	if (ENABLETASK[CONTROL]) {
//		C_Init();
		rt_task_start(&TaskControl, &T_Monitor, CONTROL);
	}
	if (ENABLETASK[ACTUATOR]) {
//		A_Init();
		rt_task_start(&TaskActuator, &T_Monitor, ACTUATOR);
	}
	if (ENABLETASK[COMM]) {
//		Comm_Init();
		rt_task_start(&TaskComm, &T_Monitor, COMM);
	}
	if (ENABLETASK[TEST])
		rt_task_start(&TaskTest, &T_Monitor, TEST);
}

void Start_Comm()
{
	if (ENABLETASK[COMM]) {
//		Comm_Init();
		rt_task_start(&TaskComm, &T_Monitor, COMM);
	}
}

void T_CleanUp (void)
{
	rt_task_delete(&TaskControl);
	rt_task_delete(&TaskSensor);
	rt_task_delete(&TaskActuator);
	rt_task_delete(&TaskComm);
	rt_task_delete(&TaskTest);
}

void T_PrintTaskName()
{
	RT_TASK *curtask;
	RT_TASK_INFO curtaskinfo;

	curtask=rt_task_self();

	rt_task_inquire(curtask,&curtaskinfo);
	rt_printf("%s ",curtaskinfo.name);
}

void T_Monitor(int functionID)
{
	//RTIME é um unsigned long long
	RTIME start, end, duration, idle, period;

	//Indicador de overuns
	unsigned long overs;

	//Valores precisam ser em Ns!
	period = PERIOD[functionID]*NS2MS;

	rt_task_set_periodic(NULL, TM_NOW, period);
	end = rt_timer_read();
	rt_task_wait_period(&overs);

	while (1) {
		if (MONITORING) {
			start = rt_timer_read();
			idle = start-end;
			T_PrintTaskName();
			rt_printf(":\t%ld.%06ld ms\t%6lld.%03lld us\t\"%d\"\n",
					(long) duration / NS2MS,
					(long) duration % NS2MS,
					(signed long long)(duration+idle-period)/ NS2US, //conversion to us
					(duration+idle-period) % NS2US,
					overs);
		}
		pfunction[functionID]();
		if (MONITORING) {
			end = rt_timer_read();
			duration = end-start;
		}
		rt_task_wait_period(&overs);
		if(overs>0) rt_printf("!\n");
	}
	return;
}

int T_Test(){
//	int l;
//	for(l = 0; l < 5; l++){
//
//	}
//	usleep(500);
	return 0;
}
