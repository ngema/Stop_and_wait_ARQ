//RECIEVER 
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<arpa/inet.h>


//SERVER  PROGRAM
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
            int k=0;
	   struct sockaddr_in  serverAddr,newAddr;
	   char buffer[1024];
	   socklen_t addr_size;
	   
	   sockfd = socket(AF_INET,SOCK_DGRAM,0);//UDP SOCKET
	   
	   int frame_id=0;
	   Frame frame_recv;
	   Frame frame_send;
	   
	   memset(&serverAddr,'\0',sizeof(serverAddr)); //filling the structure with the NULL values
	   serverAddr.sin_family= AF_INET;       //filling data  in serverAddr structure
	   serverAddr.sin_port=htons(port);
	   serverAddr.sin_addr.s_addr=inet_addr("127.0.0.1"); //you can put your own serverAddr
	   
	   bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
	   addr_size=sizeof(newAddr);
	   
	 //  recvfrom(sockfd,buffer,1024,0,(struct sockaddr*)&newAddr,&addr_size);
	  // printf("%s", buffer);
	  
	  while(1){
		  int f_recv_size=recvfrom(sockfd,&frame_recv,sizeof(Frame),0,(struct sockaddr*)&newAddr,&addr_size);
		  if (f_recv_size>0 &&frame_recv.frame_kind ==1 && frame_recv.sq_no==frame_id){
			  printf("[+]Frame recieved: %s\n", frame_recv.packet.data);
		  
		  
		  //sending acknowledgement
		  
		  frame_send.sq_no=0;
		  frame_send.frame_kind=0;
		  frame_send.ack=(frame_recv.sq_no+1);
		  
		  sendto(sockfd,&frame_send,sizeof(Frame),0,(struct sockaddr*)&newAddr,addr_size);
		  printf("[+]ACK sent\n");
                  timer();
		  }
		  else{
			  printf("[+] frame not recieved and timeout\n");
			  
			  
		  //sending NACK
		  
		  frame_send.sq_no=0;
		  frame_send.frame_kind=0;
		  frame_send.ack=(frame_recv.sq_no+1);
		  
		  sendto(sockfd,&frame_send,sizeof(Frame),0,(struct sockaddr*)&newAddr,addr_size);
		  printf("[+]NACK sent\n");
		  }
		  
		  frame_id++;
             
	  }
	  close(sockfd);
}
