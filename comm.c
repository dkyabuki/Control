#include "comm.h"

int Comm_Init()
{
	printf("Inicializando socket para comunicação...\n");
	if(( socketId = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
	{
		perror("Cannot create socket");
		return 0;
	}
	printf("Concluído!\n");
	printf("Configurando endereço IP e porta para comunicação UDP...\n");
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_port = htons(45454);

	if(inet_aton(SERVER, &address.sin_addr) == -1)
	{
		perror("inet_aton failed");
		return 0;
	}
	printf("Concluído!\n");
	printf("Conectando socket ao endereço fornecido...\n");
	if(bind(socketId, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("Bind failed");
		return 0;
	}
	printf("Concluído!\n");
	printf("Enviando mensagem de teste...\n");
	message = (struct commdata *)(malloc(sizeof(struct commdata)));
	message->position = 0;
	message->torque = 0;
	message->type = 0;
	message->time = 0;
	if(sendto(socketId, message, sizeof(message), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		perror("sendto failed");
		return 0;
	}
	printf("Concluído!\n");
	printf("Abrindo stream de arquivo para armazenamento de dados...\n");
	if((file = fopen("new_test.txt","w")) == NULL)
	{
		printf("Não foi possível abrir o arquivo!");
		return 0;
	}
	printf("Concluído!\n");
	printf("A configuração da comunicação UDP foi concluída!\n");

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
	memset((char *)message, 0, sizeof(message));
	message->type = 1;
	message->position = POSITION[2];
	message->torque = TORQUE[2];
	message->time = 0;
//	char *aux;
//	aux = message;
//	char *code;
//	code = malloc(3);
//	code = "01";
//	memcpy(aux, code, strlen(code));
//	aux += 2;
//	*aux = '|';
//	aux++;
//	memcpy(aux, &POSITION[2], sizeof(double));
//	aux += sizeof(double);
//	*aux = '|';
//	aux++;
//	memcpy(aux, &TORQUE[2], sizeof(double));
//	aux += sizeof(double);

	if(sendto(socketId, message, sizeof(message), 0, (struct sockaddr *)&address, sizeof(address)) < 0)
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
