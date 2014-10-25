#ifndef TASKS_H_
#define TASKS_H_

#include "global.h"
#include "include.h"

#include "control.h"
#include "sensor.h"
#include "actuator.h"
#include "comm.h"

//Declaração das Tasks
RT_TASK TaskSensor, TaskControl, TaskActuator, TaskComm, TaskTest;

//Período das Tasks [ms]
int PERIOD[NUMBEROFTASKS];
int PRIORITY[NUMBEROFTASKS];
int ENABLETASK[NUMBEROFTASKS];

//Array de Funções
int (*pfunction[NUMBEROFTASKS])();

void T_Init();
void T_Kill();
void T_Xenomai();
void T_Create();
void T_Start();
void Start_Comm();
void T_Monitor(int);
void T_CleanUp ();
void T_PrintTaskName();
int T_Test();

#endif /* TASKS_H_ */
