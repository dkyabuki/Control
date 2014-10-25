#ifndef COMM_H_
#define COMM_H_

#include "global.h"
#include "include.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define SERVER "127.0.0.1"

/******************************************************/
/*                  TASK VARIABLES                    */
/******************************************************/
int socketId;

//Structs
#pragma pack(push, back)
#pragma pack(1)
struct commdata
{
	unsigned short int type;
	double position;
	double torque;
	double time;
} *message;
#pragma pack(pop, back)

struct sockaddr_in address;

//File management
FILE *file;
int lineCount;

/******************************************************/
/*                     FUNCTIONS                      */
/******************************************************/
//Task functions
int Comm_Init();
int Comm_Kill();
int Comm_Core();

//File management functions
void saveToFile (FILE *f);
void closeFile (FILE *f);

#endif /* COMM_H_ */
