#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

#define MAX_SEND_SIZE 100

struct msgbuf{
	long mtype;
	char mtext[MAX_SEND_SIZE];
};
int msgqueue_id;
const int ESC_CODE=27;
const int RETURN_CODE=10;

void read_message(int gid,struct msgbuf *bufs,long type)
{
	bufs->mtype=0;
	msgrcv(gid,(struct msgbuf *)bufs,MAX_SEND_SIZE,0,0);
	printf("%s",bufs->mtext);
}
void send_message(int gid,struct msgbuf *bufs,long type,char *text)
{
	bufs->mtype=type;
	
	strcpy(bufs->mtext,text);
	printf("%s\n",bufs->mtext);
	//printf("%s\n%ld\n%d\n",bufs->mtext,bufs->mtype,gid);
	if (msgsnd(gid,(struct msgbuf *)bufs,strlen(text)+1,0)==-1){
		printf("Message send error\n");
		exit(-1);
	}
}
int main (int argc,char *argv[]){
	key_t key;
	struct msgbuf msg_snd, msg_rcv;
	int main_id;

	if (argv[1]==NULL){
		printf("Error: Write down your login\n");
		exit(-1);
	}
	if (strlen(argv[1])>20)
	{
		printf("Login is too long\n");
		exit(-1);
	}
	key=ftok(".",getpid());
	if ((msgqueue_id=msgget(key,IPC_CREAT|0666))==-1){
		printf("Message queue creating error\n");
		exit(-1);	
	}
	if ((main_id=msgget(1234,IPC_CREAT|0666))==-1){
		printf("Не удалось подключиться к серверу");
		exit(-1);
	}
	int c;
	int i=0;
	char buf[MAX_SEND_SIZE];
	snprintf(buf,MAX_SEND_SIZE,"%d",msgqueue_id);
	printf("%s\n",buf);	
	send_message(main_id,&msg_snd,1,buf);
	printf("%s: ",argv[1]);
	pid_t pid;
	if (pid=fork())
	while(1)
	{
		read_message(msgqueue_id,&msg_rcv,4);
		printf("%s\n",msg_rcv.mtext);
	}
	while(c!=ESC_CODE)
	{
		c=getchar();
		if (i<MAX_SEND_SIZE-20)
			buf[i++]=c;
		if (c=='\n'){
			//Send a message
			send_message(main_id,(struct msgbuf *) &msg_snd,2,buf);
			i=0;
			printf("%s: ",argv[1]);
		}
	}
	kill(pid,SIGKILL);
	snprintf(buf,MAX_SEND_SIZE,"%d",msgqueue_id);
	send_message(main_id,&msg_snd,3,buf);
	msgctl(msgqueue_id,IPC_RMID,0);
	exit(0);	
}

