#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

#define MAX_SEND_SIZE 100
#define ESC_CODE 27

struct msgbuf {
	long mtype;
	char mtext[MAX_SEND_SIZE];
};

void send_message(int qid, struct msgbuf *qbuf, long type, char *text) 
{
	qbuf->mtype = type;
	strcpy(qbuf->mtext, text);
	printf("Send %s\n",qbuf->mtext);
	if ((msgsnd(qid, (struct msgbuf *) qbuf, strlen(qbuf->mtext) + 1, 0)) == -1) 
	{
		perror("Ошибка отправки сообщения");
		exit(-1);
	}
}

void read_message(int qid, struct msgbuf *qbuf, long type) 
{
	qbuf->mtype = type;
	msgrcv(qid, (struct msgbuf *) qbuf, MAX_SEND_SIZE,type, 0);
	printf("Get %s\n",qbuf->mtext);
}

int main(int argc, char *argv[]) 
{
	key_t key;
	int msgqueue_id;
	struct msgbuf qbuf;
	struct msgbuf outbuf;
	
	/* Create unique key via call to ftok() */
	key = 1234;

	if ((msgqueue_id = msgget(key, IPC_CREAT | 0660)) == -1) 
	{
		perror("Ошибка создания очереди");
		exit(-1);
	}
	char tmp[5];
	send_message(msgqueue_id, (struct msgbuf *) &qbuf, 1, 
"4");

  	int c = 0;
  	pid_t pid ;
	if ((pid=fork()) == 0)
	while(1)
  	{
	 	read_message(msgqueue_id, &outbuf, 2);
		char msg[MAX_SEND_SIZE];
		strcpy(msg,outbuf.mtext);

	  	read_message(msgqueue_id, &qbuf, 1);
		int last_client=atoi(qbuf.mtext);
	  	sprintf(tmp, "%d", last_client);
	  	send_message(msgqueue_id, (struct msgbuf *) &qbuf, 1, tmp);
		int i;
		for (i = 4; i < last_client; i++)
			send_message(msgqueue_id, (struct msgbuf *) &outbuf, i, msg);
		}

  	while (c!=ESC_CODE){
		printf("Сервер работает. Нажмите ESQ для прекращения работы.");
		c=getchar();
	}
	
	printf("Cервер закрыт\n");
	kill(pid,SIGKILL);
	msgctl(msgqueue_id,IPC_RMID,0);
	exit(0);
}

