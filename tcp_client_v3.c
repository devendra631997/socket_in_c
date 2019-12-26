#include<stdio.h> //for printf and scanf
#include<sys/types.h> //socket system call
#include<sys/socket.h> //socket system call
#include<strings.h> //bzero API
#include<unistd.h> //read system call
#include<stdlib.h> //for exit command
#include<netinet/in.h> //structure member - man 7 ip
#define PORT 4000
#define BUFFER_SIZE 1024
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

serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //inet_addr converts IPV4 dotted decimal format into binary (network byte order)
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


cpid = fork();
if (cpid == 0){
while(1)
{
bzero(sendbuffer,sizeof(sendbuffer));
int buffer_count=0;
while((sendbuffer[buffer_count++]=getchar())!='\n');
write(sockfd,sendbuffer,sizeof(sendbuffer)); //write the data in buffer to server
}}
else {
while(1){
bzero(recvbuffer,sizeof(recvbuffer));
read(sockfd,recvbuffer,sizeof(recvbuffer)); //receive data in buffer from server
printf("Data from server:%s",recvbuffer);
}}
close(sockfd);
}
