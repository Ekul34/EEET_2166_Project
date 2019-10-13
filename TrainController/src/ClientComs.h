/*
 * ClientComs.h
 *
 *  Created on: 13Oct.,2019
 *      Author: Shenal
 */

//for now the function just accepts n,e or x

//do these need to be included here as well if they are already in the main C file ?
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/iofunc.h>
#include <sys/netmgr.h>


#ifndef CLIENTCOMS_H_
#define CLIENTCOMS_H_
	int serverPID;
	int serverCHID;
#define BUF_SIZE 100

typedef struct
{
	struct _pulse hdr; // Our real data comes after this header
	int ClientID; // our data (unique id from client)
    int data;     // our data
} my_data;

typedef struct
{
	struct _pulse hdr; // Our real data comes after this header
    char buf[BUF_SIZE];// Message we send back to clients to tell them the messages was processed correctly.
} my_reply;

typedef struct
{
	int data1;//serverPID
	int data2;//serverCHID
} input_data;


//pass in host name to construct directory
int Client_sttach(string hostname,string filename)
{
	char hostname[]="EAf46089";
	char filename[]="server.info";
	FILE *fptr;
	int serverPID;
	int serverCHID;

	char dir[]="/net/";
	char dir2[]="/tmp/file/";
	strcat(dir,hostname);
	strcat(dir,dir2);
	strcat(dir,filename);


	   if ((fptr = fopen(dir,"r")) == NULL)
	   {
		   //printf("",fptr);
		   printf("Error! opening file");

		   // Program exits if the file pointer returns NULL.
		   exit(1);
	   }

	   fscanf(fptr,"%d\n", &serverPID);
	   fscanf(fptr,"%d", &serverCHID);

	   //printf("Value of n=%d", num);
	   fclose(fptr);

	   msg.ClientID = 500;

	      int server_coid;
	      int index = 0;

	  	printf("   --> Trying to connect (server) process which has a PID: %d\n",   tdc->data1);
	  	printf("   --> on channel: %d\n\n", tdc->data2);

	  	// set up message passing channel
	      server_coid = ConnectAttach(1, tdc->data1, tdc->data2, _NTO_SIDE_CHANNEL, 0);
	  	if (server_coid == -1)
	  	{
	          printf("\n    ERROR, could not connect to server!\n\n");
	          pthread_exit( EXIT_FAILURE );
	  	}
	  	input_data data2;

	  	//server_coid=data2;
	      printf("Connection established to process with PID:%d, Ch:%d\n", tdc->data1, tdc->data2);
	      printf("\nserver_coid:%d\n",server_coid);

	      // We would have pre-defined data to stuff here
	      msg.hdr.type = 0x00;
	      msg.hdr.subtype = 0x00;
	      char buf[BUF_SIZE] = {};
	      while(1)
	      {
	      	printf("enter char to be sent\n");
	      	scanf("%c",&buf);
	      	if(buf[0] == 'n')
	      	{
	      		msg.data=1;
	      		// the data we are sending is in msg.data
	      		printf("Client (ID:%d), sending data packet with the integer value: %d which is character n \n", msg.ClientID, msg.data);
	      		fflush(stdout);
	      		 if (MsgSend(server_coid, &msg, sizeof(msg), &reply, sizeof(reply)) == -1)
	      		        {
	      			 	 	 printf("\nError at line 69\n");

	      		            printf(" Error data '%d' NOT sent to server\n", msg.data);
	      		            	// maybe we did not get a reply from the server
	      		            break;
	      		        }
	      		        else
	      		        { // now process the reply
	      		            printf("   -->Reply is: '%s'\n", reply.buf);
	      		        }
	      	}
	      	if(buf[0] == 'e')
	      	{
	      		msg.data=2;
	      		// the data we are sending is in msg.data
	      		printf("Client (ID:%d), sending data packet with the integer value: %d which is character e \n", msg.ClientID, msg.data);
	      		fflush(stdout);
	      		 if (MsgSend(server_coid, &msg, sizeof(msg), &reply, sizeof(reply)) == -1)
	      		        {
	      		            printf(" Error data '%d' NOT sent to server\n", msg.data);
	      		            	// maybe we did not get a reply from the server
	      		            break;
	      		        }
	      		        else
	      		        { // now process the reply
	      		            printf("   -->Reply is: '%s'\n", reply.buf);
	      		        }
	      	}
	      	if(buf[0] == 'x')
	      	{
	      		msg.data=3;
	      		// the data we are sending is in msg.data
	      		printf("Client (ID:%d), sending data packet with the integer value: %d which is character e \n", msg.ClientID, msg.data);
	      		fflush(stdout);

	      		 if (MsgSend(server_coid, &msg, sizeof(msg), &reply, sizeof(reply)) == -1)
	      		 {
	      		     printf(" Error data '%d' NOT sent to server\n", msg.data);
	      		        	// maybe we did not get a reply from the server
	      		     break;
	      		 }
	      		 break;
	      	}
	      	buf[0] = 0;
	      }




	   return servePID, serverCHID,msg.data;//if we need to do something with this
}


#endif /* CLIENTCOMS_H_ */
