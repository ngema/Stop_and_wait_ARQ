#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>

typedef struct packet{
	char data[1024];
	
}Packet;
typedef struct frame{
	int frame_kind;
	int sq_no;//sequence number 
	int ack;//acknowledgement
	Packet packet; // this is the data
	
}Frame;

void main(int argc ,char **argv){
       if(argc !=2){
		   printf("Usage:%s <port>\n",argv[0]);
		   exit(0);
	   }
	   int port=atoi(argv[1]);
	   int sockfd;
	   struct sockaddr_in  serverAddr,newAddr;
	   char buffer[1024];
	   socklen_t addr_size;
	   int k=0;
	   int frame_id=0;
	   Frame frame_send;//frame to send
	   Frame frame_recv;//frame to recieve
	   
	   int ack_recv=1; //used to reflect if ack is recieved
	   float transmission_delay;
	   float bandwidth=0.01;
	   float data_size=7;
	   
	   sockfd = socket(AF_INET,SOCK_DGRAM,0);//UDP SOCKET
	   
	   memset(&serverAddr,'\0',sizeof(serverAddr)); //filling the structure with the NULL values
	   serverAddr.sin_family= AF_INET;       //filling data  in serverAddr structure
	   serverAddr.sin_port=htons(port);
	   serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1"); //you can put your own serverAddr
	   
	   //stpcpy(buffer,"Hello udp serverAddr\n");
	   //sendto(sockfd,buffer,1024,0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	  
    //0-ACK
    // 1-NACK	
	   while(1){
		   if(ack_recv=1){
		   frame_send.sq_no=frame_id;
		   frame_send.frame_kind=1;
		   frame_send.ack=0;
		   
		   printf("Enter Data: ");
		   scanf("%s",buffer);
		   strcpy(frame_send.packet.data,buffer);
		  
		   sendto(sockfd,&frame_send,sizeof(Frame),0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));//sending frame
		   printf("[+]Frame sent\n");
		   
		    transmission_delay=(data_size/bandwidth) ;
		   printf("transmission delay: %f ms\n",transmission_delay);
	   }
	   
	   int addr_size=sizeof(serverAddr);
	   int f_recv_size=recvfrom(sockfd,&frame_recv,sizeof(Frame),0,(struct sockaddr*)&serverAddr,&addr_size);
	   
	   if(f_recv_size>0 &&frame_recv.sq_no==0 && frame_recv.ack==frame_id+1){
		   printf("[+]ACK recieved\n");
		   ack_recv=1;
	   }
	   else{
		   printf("[+] NACK recieved\n");
		   ack_recv=0;
	   }
	   frame_id++; //send the second frame 
	  
	   }
	   close(sockfd);

}
