#include "comm.h"

int Comm_Init()
{
//	printf("Inicializando socket para comunicação...\n");
//	if(( socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
//	{
//		perror("Cannot create socket");
//		return 0;
//	}
//	printf("Concluído!\n");
//	printf("Configurando endereço IP e porta para comunicação UDP...\n");
//	memset((char *)&address, 0, sizeof(address));
//	address.sin_family = AF_INET;
//	address.sin_port = htons(45454);
//	address.sin_addr.s_addr = inet_addr(SERVER);
//
//	printf("Concluído!\n");
//	printf("Conectando socket ao endereço fornecido...\n");
//	if(bind(socketId, (struct sockaddr *)&address, sizeof(address)) < 0)
//	{
//		perror("Bind failed");
//		return 0;
//	}
//	printf("Concluído!\n");
//	printf("Enviando mensagem de teste...\n");
//	message = (struct commdata *)(malloc(sizeof(struct commdata)));
//	memset(message, 0, sizeof(message));
//	message->position = 0;
//	message->torque = 0;
//	message->type = 0;
//	message->time = 0;
//	strcpy(message->msg,"Hello world!");
//	message->size = strlen(message->msg);
//	if(sendto(socketId, message, sizeof(message), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
//	{
//		perror("sendto failed");
//		return 0;
//	}
//	printf("Concluído!\n");


	printf("Digite o nome do arquivo para salvar os dados (com o formato): ");
	char filename[20];
	scanf("%s", filename);
	printf("Abrindo stream de arquivo para armazenamento de dados...\n");
	if((file = fopen(filename,"w")) == NULL)
	{
		printf("Não foi possível abrir o arquivo!");
		return 0;
	}
	printf("Concluído!\n");
	printf("A configuração da comunicação UDP foi concluída!\n");
	savingData = FALSE;
	lineCount = 0;
	return 0;
}

int Comm_Kill()
{
	free(message);
	return 0;
}

int Comm_Core()
{
//	memset((char *)message, 0, sizeof(message));
//	message->type = 1;
//	message->position = 0;
//	message->torque = 0;
//	message->time = 0;
//	memset(message->msg, 0, sizeof(message->msg));
//	message->size = 0;
////	char *aux;
////	aux = message;
////	char *code;
////	code = malloc(3);
////	code = "01";
////	memcpy(aux, code, strlen(code));
////	aux += 2;
////	*aux = '|';
////	aux++;
////	memcpy(aux, &POSITION[2], sizeof(double));
////	aux += sizeof(double);
////	*aux = '|';
////	aux++;
////	memcpy(aux, &TORQUE[2], sizeof(double));
////	aux += sizeof(double);
//
//	if(sendto(socketId, message, sizeof(message), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
//	{
//		perror("sendto failed");
//		return 0;
//	}

	if(savingData)
	{
		saveToFile (file);
		lineCount++;
	}


//	if(lineCount >= 0 && lineCount < 16000){ 	//salva
//		saveToFile (file);
//		lineCount++;
//	} else if(lineCount == 16000){ 		//para
//		closeFile(file);
//		lineCount++;
//	} else { 					//finish
//		rt_printf("\tDONE!\n");
//	}
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

void startSaving(){
	rt_printf("Começou!\n");
	dscDIOOutputBit(dscb, 1, 0, 1);
	savingData = TRUE;
}

void finishSaving(){
	rt_printf("Terminou!\n");
	dscDIOOutputBit(dscb, 1, 0, 0);
	savingData = FALSE;
	closeFile(file);
}
