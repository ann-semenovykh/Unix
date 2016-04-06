#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

#define MAX_SEND_SIZE 80

struct msgbuf{
	long mtype;
	char mtext[MAX_SEND_SIZE];
}msg_snd,msg_rcv;
int msgqueue_id;
const int ESC_CODE=27;
const int RETURN_CODE=10;
int main (int argc,char *argv[]){
	key_t key;
	long my_id=(long)random();
	if (argv[1]==NULL){
		printf("Error: Write down your login\n");
		exit(-1);
	}
	key=ftok(".",'m');
	if ((msgqueue_id=msgget(key,IPC_CREAT|0666))==-1){
		printf("Message queue creating error\n");
		exit(-1);	
	}
	
	char c='0';
	int i=0;
	char *buf;
	buf=(char*)malloc(sizeof(char)*MAX_SEND_SIZE);
	printf("%s: ",argv[1]);
	while (c!='\e'){
		c=getchar();
		if (i<MAX_SEND_SIZE)
			buf[i++]=c;	
		//else
		//	printf("Very big message!\n");
		if (c=='\n'){
			//Send a message
			send_message(msgqueue_id,(struct msgbuf *) &msg_snd,my_id,buf);
			
		}
	}
	free(buf);	
}

void read_message(int gid,struct msgbuf *bufs,long type)
{
	bufs->mtype=0;
	msgrcv(gid,(struct msgbuf *)bufs,MAX_SEND_SIZE,0,0);
	printf("%s",bufs->mtext);
}
void send_message(int gid,struct msgbuf *bufs,long type,char *text)
{
	bufs->mtype=0;
	strcpy(bufs->mtext,text);
	if (msgsnd(gid,(struct msgbuf *)bufs,strlen(text)+1,0)==-1){
		printf("Message send error\n");
		exit(-1);
	}
}
