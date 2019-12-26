#include<stdio.h> //for printf and scanf
#include<sys/types.h> //socket system call
#include<sys/socket.h> //socket system call
#include<strings.h> //bzero API
#include<unistd.h> //read system call
#include<stdlib.h> //for exit command
#include<netinet/in.h> //structure member - man 7 ip

    char sendbuffer[1024];
	char recvbuffer[1024];
void error(char *msg)

{
    perror(msg);
    exit(1);
}

int main(int argc,char *argv[])
{
    int sockfd; //first socket descriptor made by socket()
    int connfd; //second socket descriptor made by accept()
    int portnumber, clilen; //server portnuber & client address len declaration
    char sendbuffer[1024];
	char recvbuffer[1024];
pid_t cpid;
    struct sockaddr_in serv_addr, cli_addr; //server/client structure declaration
    int readbytes; //bytes read at server end written by client   

    if(argc<2)
    {
        error("Error !! Enter the Port Number while running the program -./a.out <portnumber>\n");
    }

    sockfd = socket(AF_INET,SOCK_STREAM,0); //sockfd is socket descriptor
    if(sockfd<0) // condition check for socket return type
    {
        error("Socket is not created");
    }

    else 
    {
        printf("Socket is created successfully\n");
    }

    bzero((char *)&serv_addr,sizeof(serv_addr)); //clean the structure buffer with bzero to avoid any garbage value 

    portnumber = atoi(argv[1]); //argv[1] is first argument & port number is entered from command line

    serv_addr.sin_family=AF_INET; //initialize the members of structures

    serv_addr.sin_addr.s_addr=INADDR_ANY; //INADDR_ANY binds to all available interfaces of host machine
    serv_addr.sin_port = htons(portnumber); // htons - host number byte order(BigEndian)

    int ret = bind(sockfd,(struct sockaddr*) &serv_addr,sizeof(serv_addr)); //bind socket with server address

    if(ret<0)
    {
        error("Error in binding the socket");
    }
    else
    {
        printf("Socket Binding is Successful !!\n");
    }

    int backlog =5; //listen to maximum backlog connections 
    int ret1=listen(sockfd,backlog);
    
    if(ret1<0)
    {
        error("listen failed to accept requests to keep it in pending queues");
    }
    else
    {
        printf("server started successfully\n");
    }

    clilen=sizeof(cli_addr); //calculate the size of client structure
    connfd=accept(sockfd,(struct sockaddr *)&cli_addr,&clilen); //accept system call - to fetch pending requests from the queues

    if(connfd<0)
    {
        error("error in accepting the request");
    }
    else
    {
        printf("request accepted successfully with connefd: %d\n",connfd);
    }

cpid =fork();
if (cpid==0){

    while(1)
    {
        bzero(recvbuffer,sizeof(recvbuffer)); //clean the buffer to avoid any garbage value
        readbytes=read(connfd,recvbuffer,sizeof(recvbuffer)); //read data from the client application

        if(readbytes<0)
        {
            error("No data is read from the client");
        }

        printf("The msg received from client: %s\n",recvbuffer);
        char successmsg[1024]="Cheers!! Recieved the message from the client";
        int writebytes;
        writebytes=write(connfd,successmsg,sizeof(successmsg));

        if(writebytes < 0 ) 
        {
            error("Error in writing the data");   
        }
    }}

else{
while(1)
{
bzero(sendbuffer,sizeof(sendbuffer));
int buffer_count=0;
while((sendbuffer[buffer_count++]=getchar())!='\n');
write(connfd,sendbuffer,sizeof(sendbuffer));
}
}
    close(sockfd); //close the socket file descriptor
    
}
