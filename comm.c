#include "comm.h"

#define BAUDRATE B19200
#define MODEMDEVICE "/dev/ttyS0"
#define _POSIX_SOURCE 1

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
//	char buf[255];
	printf("Inicializando porta serial para comunicação...\n");
	fd = open_port();
	if(fd == -1)
	{
		printf("Não foi possível abrir a porta serial!\n");
		return 0;
	}

	tcgetattr(fd, &options);
	old = options;
	cfsetispeed(&options, BAUDRATE);
	cfsetospeed(&options, BAUDRATE);
	options.c_cflag |= (CLOCAL | CREAD);
	options.c_cflag &= ~PARENB;
	options.c_cflag &= ~CSTOPB;
	options.c_cflag &= ~CSIZE;
	options.c_cflag |= CS8;
	options.c_cflag &= ~CRTSCTS;
	options.c_iflag = IGNPAR | ICRNL;
	options.c_iflag &= ~(IXON | IXOFF | IXANY);
	options.c_oflag = 0;
	options.c_lflag = (ICANON | ECHO | ECHOE);
	memset(options.c_cc, 0, sizeof(options.c_cc));
	options.c_cc[VEOF] = 4;
	options.c_cc[VMIN] = 1;
	tcflush(fd, TCIFLUSH);
	tcsetattr(fd, TCSANOW, &options);
	printf("A configuração da porta serial foi concluída!\n");

	printf("Digite o nome do arquivo para salvar os dados (com o formato): ");
	char filename[20];
	scanf("%s", filename);
	printf("Abrindo stream de arquivo para armazenamento de dados...\n");
	if((file = fopen("startend.txt","w")) == NULL)
	{
		printf("Não foi possível abrir o arquivo!\n");
		return 0;
	}
	printf("Concluído!\n");
	savingData = FALSE;
	lineCount = 0;
	return 0;
	running = false;
}

int Comm_Kill()
{
	free(message);
	tcsetattr(fd, TCSANOW, &old);
	return 0;
}

int Comm_Core()
{
	if(running == true)
	{


		if(savingData)
		{
			saveToFile (file);
			lineCount++;
		}
	}
	else
	{
		memset(buff, 0, sizeof(buff));
		int res;
		char* aux;
		aux = buff;

		while((res = read(fd, aux, buff + sizeof(buff) - aux -1)) > 0)
		{
			aux += res;
			if(aux[-1] == '\0')
				break;
		}
		printf("mensagem recebida: ");
		message = (struct commdata *)buff;
		if(message->start == ':')
			if(message->id == 'A')
				switch(message->command[0])
				{
				case ('S'):
					printf("mensagem de config. - ");
					switch(message->command[1])
					{
					case('T'):
							printf("tarefas\n");
							break;
					}
					break;
				}
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

int open_port()
{
	int fd;
	fd = open(MODEMDEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
	if (fd == -1)
	{
		printf("open_port: Unable to open port\n");
	}
	else
		fcntl(fd, F_SETFL, FNDELAY);
	return (fd);
}
