#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

#define MAX_SEND_SIZE 10
#define MAX_NAME_LENGTH 7
#define ESC_CODE 27


int client_type;

struct msgbuf {
	long mtype;
	char mtext[MAX_SEND_SIZE+MAX_NAME_LENGTH];
};

void send_message(int qid, struct msgbuf *qbuf, long type, char *text)
{
	qbuf->mtype = type;
	strcpy(qbuf->mtext, text);
	//printf("Send %s\n",qbuf->mtext);
	if ((msgsnd(qid, (struct msgbuf *) qbuf, strlen(qbuf->mtext) + 1, 0)) == -1) 
	{
		perror("msgsnd");
		exit(1);
	}
}

void read_message(int qid, struct msgbuf *qbuf, long type, int flag) 
{
	qbuf->mtype = type;
	msgrcv(qid, (struct msgbuf *) qbuf, MAX_SEND_SIZE+MAX_NAME_LENGTH+2, type, flag);
	//printf("Get %s\n",qbuf->mtext);
}

int main(int argc, char *argv[]) 
{
	key_t key;
	int msgqueue_id;
	struct msgbuf qbuf,msg_snd;
	pid_t pid;
	/* Create unique key via call to ftok() */
	key = 1234;

	if (argv[1]==NULL){
		printf("Ошибка: введите свой логин\n");
		exit(-1);
	}
	if (strlen(argv[1])>MAX_NAME_LENGTH-2)
	{
		printf("Логин слишком длинный\n");
		exit(-1);
	}
	char log_name[MAX_NAME_LENGTH+MAX_SEND_SIZE];
	char name[MAX_NAME_LENGTH];
	strcat(name,argv[1]);
	strcat(name,": ");
	strcpy(log_name,name);
	if ((msgqueue_id=msgget(1234,IPC_CREAT|0666))==-1){
		printf("Не удалось подключиться к серверу");
		exit(-1);
	}

  	int c = 0;
  	int i = 0;
  	char tmp[MAX_SEND_SIZE+MAX_NAME_LENGTH];
  	read_message(msgqueue_id, &qbuf, 1, 0);
	client_type=atoi(qbuf.mtext);
  	sprintf(tmp, "%d", client_type + 1);
  	send_message(msgqueue_id, (struct msgbuf *) &qbuf, 1, tmp);

  	char buf[MAX_SEND_SIZE];
	if ((pid=fork()) == 0)
		while(1)
			{
			struct msgbuf in;
			read_message(msgqueue_id, &in, client_type, 0);			
			printf("%s\n",in.mtext);
		}
	while (c != ESC_CODE) 
	{
		
		c = getchar();
		if (i < MAX_SEND_SIZE) 
			buf[i++] = c; 
		if (c == '\n') 
		{
			buf[i] = 0;
			/*if (i>MAX_SEND_SIZE-MAX_NAME_LENGTH)
				printf("Сообщение слишком длинное\n");
			//Send a message*/
			send_message(msgqueue_id,(struct msgbuf *) &msg_snd,2,strcat(log_name,buf));
			i=0;
			strcpy(log_name,name);
			buf[i]=0;
		}
			
		
	}
	kill(pid,SIGKILL);
	//strcpy(tmp,"DELETE");
	//send_message(msgqueue_id,(struct msgbuf *)&qbuf,3,tmp);
	exit(0);
}

