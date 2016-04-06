#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <signal.h>

const int ESC_CODE = 27;

#define MAX_SEND_SIZE 80

struct msgbuf{
	long mtype;
	char mtext[MAX_SEND_SIZE];
};
long users[5];
int count;
	int gid;


void read_message(long type) {
	/* Read a message from the queue */
	struct msgbuf qbuf;
	qbuf.mtype = type;
	msgrcv(gid, (void *) &qbuf, MAX_SEND_SIZE, type, 0);
	switch (type){
	case 1:printf("User added: %s\n", qbuf.mtext);
		users[count]=atol(qbuf.mtext);
		count++;
		printf("%ld\n",atol(qbuf.mtext));
	break;
	case 3: printf ("User removed: %s\n", qbuf.mtext);
		count--;
	break;
	case 2: 
		qbuf.mtype=4;
		int i;
		for (i=0;i<=count;i++){
			msgsnd(users[i],(void *)&qbuf,strlen(qbuf.mtext)+1,0);
		printf("Сообщение отправлено %ld\n",users[i]);}
	break;
	}
}
void main(int argc, char *argv[])
{
	key_t key;
	pid_t pid1,pid2,pid3;
	count=0;
	key=1234;
	if ((gid=msgget(key, IPC_CREAT | 0660))==-1){
		perror("msgget");
		exit(0);
	}
	//printf("%d\n",gid);
	int c;
	if ((pid1=fork())==0)
		while (1)
			read_message(1);
	if ((pid2=fork())==0)
		while (1)
			read_message(2);
	if ((pid3=fork())==0)
		while (1)
			read_message(3);
	while (c!=ESC_CODE){
		printf("Сервер работает. Нажмите ESQ для прекращения работы.");
		c=getchar();
	}
	printf("Cервер закрыт\n");
	kill(pid2,SIGKILL);
	kill(pid1,SIGKILL);
	kill(pid3,SIGKILL);
	msgctl(gid,IPC_RMID,0);
	exit(0);	
}


