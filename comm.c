#include "comm.h"

int Comm_Init()
{
	if(( socketId = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("Cannot create socket");
		return 0;
	}
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(0);
	if(bind(socketId, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Bind failed");
		return 0;
	}
	message = (struct commdata *)(malloc(sizeof(struct commdata)));
	message->type = 0;
	message->position = 0;
	message->torque = 0;
	message->time = 0;
	if(sendto(socketId, (char *)message, sizeof(message), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("sento failed");
		return 0;
	}
	file = fopen("new_test.txt","w");
	lineCount = 0;
	return 0;
}

int Comm_Kill()
{
	return 0;
}

int Comm_Core()
{
	message->type = 1;
	message->position = POSITION[2];
	message->torque = TORQUE[2];
	message->time = 0;
	if(sendto(socketId, (char *)message, sizeof(message), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("sendto failed");
		return 0;
	}

	if(lineCount >= 0 && lineCount < 16000){ 	//salva
		saveToFile (file);
		lineCount++;
	} else if(lineCount == 16000){ 		//para
		closeFile(file);
		lineCount++;
//	} else if(lineCount <= 0){ 			//wait
//		rt_printf("\t%d \tWAITING!\n", lineCount);
//		lineCount++;
	} else { 					//finish
		rt_printf("\tDONE!\n");
	}
	return 0;
}

void saveToFile (FILE *f){
	if (file != NULL){
		fprintf(f, "%5.8f, %5.8f, %5.8f, %5.8f, %5.8f,\n", torque_reading, TORQUE[2], position_reading, POSITION[2], setpoint);
	}
}

void closeFile (FILE *f){
	if (file != NULL){
		fclose(f);
	}
}
