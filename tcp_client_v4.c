#include<stdio.h> //for printf and scanf
#include<sys/types.h> //socket system call
#include<sys/socket.h> //socket system call
#include<strings.h> //bzero API
#include<unistd.h> //read system call
#include<stdlib.h> //for exit command
#include<netinet/in.h> //structure member - man 7 ip
#define PORT 6000
#define BUFFER_SIZE 1024
#include<pthread.h>
int counter=0;
pthread_mutex_t lock;

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in serv_addr;
	    char sendbuffer[BUFFER_SIZE];
	char recvbuffer[BUFFER_SIZE];
pid_t cpid;
	
	sockfd=socket(AF_INET,SOCK_STREAM,0); //creation of Socket
if(sockfd < 0)
{
	error("Socket creation failed");
}
else
{
	printf("Socket created successfully\n");
}

bzero(&serv_addr,sizeof(serv_addr)); //clear/flush the structure memory
serv_addr.sin_family=AF_INET; // Assigning IP address and port

serv_addr.sin_addr.s_addr = inet_addr("192.168.75.56"); //inet_addr converts IPV4 dotted decimal format into binary (network byte order)
serv_addr.sin_port = htons(PORT);
int retcon = connect(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr)); //connect the client socket to server socket
if(retcon <0 )
{
	error("Connection to server socket failed");
}
else
{
	printf("Connected to server successfully\n");
}


int counter=0;
pthread_mutex_t lock;

void *count1(void *pv)
{
	//pthread_mutex_lock(&lock);
	while(1)
	{
	bzero(sendbuffer,sizeof(sendbuffer));
	int buffer_count=0;
	while((sendbuffer[buffer_count++]=getchar())!='\n');
	write(sockfd,sendbuffer,sizeof(sendbuffer)); //write the data in buffer to server
	}
	//pthread_mutex_unlock(&lock);
}

void *count2(void *pv)
{
	//pthread_mutex_lock(&lock);
	while(1)
	{
	bzero(recvbuffer,sizeof(recvbuffer));
	read(sockfd,recvbuffer,sizeof(recvbuffer)); //receive data in buffer from server
	printf("Data from server:%s",recvbuffer);
	}
	//pthread_mutex_unlock(&lock);
}

	pthread_t thread1,thread2;
	pthread_create(&thread2,NULL,count1,NULL);
	pthread_create(&thread1,NULL,count2,NULL);
	pthread_join(thread1,NULL);
	pthread_join(thread2,NULL);

close(sockfd);

}
