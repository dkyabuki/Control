#ifndef COMM_H_
#define COMM_H_

#include "global.h"
#include "include.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define SERVER "127.0.0.1"

/******************************************************/
/*                  TASK VARIABLES                    */
/******************************************************/
int socketId;
int fd;
short int savingData;
char buff[262];
bool running;

//Structs
#pragma pack(push, back)
#pragma pack(1)
struct commdata
{
	char 				start;
	char 				id;
	char 				command[2];
	unsigned char 		size;
	char       		   *data;
	char        		checksum[2];
} *message;
#pragma pack(pop, back)

//struct sockaddr_in address;
struct termios options, old;


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

int open_port();

//File management functions
void saveToFile (FILE *f);
void closeFile (FILE *f);
void startSaving();
void finishSaving();

#endif /* COMM_H_ */
