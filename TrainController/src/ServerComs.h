/*
 * ServerComs.h
 *
 *  Created on: 13Oct.,2019
 *      Author: Shenal
 */

#ifndef SERVERCOMS_H_
#define SERVERCOMS_H_

//pass hostname and fileneame with extension as string when calling function

int CreateFile(string hostname,string filename)
{
	int serverPID=0, chid=0; 	// Server PID and channel ID
		int num;
		FILE *fptr;

		serverPID = getpid(); 		// get server process ID

		// Create Channel
		chid = ChannelCreate(_NTO_CHF_DISCONNECT);
		if (chid == -1)  // _NTO_CHF_DISCONNECT flag used to allow detach
		{
		    printf("\nFailed to create communication channel on server\n");
		    pthread_exit( EXIT_FAILURE );
		}

		printf("Server Listening for Clients on:\n");
		printf("  --> Process ID   : %d \n", serverPID);
		printf("  --> Channel ID   : %d \n\n", chid);

		string dir[50]="/net/";
		String dir2[50]="/tmp/file/";
		strcat(dir,hostname);
		strcat(dir,dir2);
		strcat(dir,filename);

		fptr = fopen(dir,"w");

		if(fptr == NULL)
		{
			printf("Error!\nPlease check file directory");
			exit(1);
		 }

		fprintf(fptr,"%d\n",serverPID);
		fprintf(fptr,"%d",chid);
		printf("Data written to file in %s\n",dir);
		fclose(fptr);

		my_data msg;
		int rcvid=0, msgnum=0;  	// no message received yet
		int Stay_alive=0, living=0;	// server stays running (ignores _PULSE_CODE_DISCONNECT request)

		my_reply replymsg; 			// replymsg structure for sending back to client
		replymsg.hdr.type = 0x01;
		replymsg.hdr.subtype = 0x00;


	   	    if(msg.data == 1)
			{
				printf("Got n\n");
				ns_car = 0;
			}
			if(msg.data ==2)
			{
				printf("Got e\n");
				ew_car = 0;
			}
			if (msg.data == 3)			//once we get this message we know not to expect any more mqueue data
			{
				printf("Got x\n");
				printf("\nServer received Destroy command\n");
				// destroyed channel before exiting
				ChannelDestroy(chid);
				pthread_exit( EXIT_FAILURE );
				shortcut = 0;
			}



			 if(rcvid > 0) // if true then A message was received
				   {
					   msgnum++;

					   // If the Global Name Service (gns) is running, name_open() sends a connect message. The server must EOK it.
					   if (msg.hdr.type == _IO_CONNECT )
					   {
						   MsgReply( rcvid, EOK, NULL, 0 );
						   printf("\n gns service is running....");
						   continue;	// go back to top of while loop
					   }

					   // Some other I/O message was received; reject it
					   if (msg.hdr.type > _IO_BASE && msg.hdr.type <= _IO_MAX )
					   {
						   MsgError( rcvid, ENOSYS );
						   printf("\n Server received and IO message and rejected it....");
						   continue;	// go back to top of while loop
					   }

					   // A message (presumably ours) received

					   // put your message handling code here and assemble a reply message
					   sprintf(replymsg.buf, "Message %d received", msgnum);
					  // printf("Server received data packet with value of '%d' from client (ID:%d), ", msg.data, msg.ClientID);
						   fflush(stdout);


						   	   	    if(msg.data == 1)
						   			{
						   				printf("Got n\n");
						   				ns_car = 0;
						   			}
						   			if(msg.data ==2)
						   			{
						   				printf("Got e\n");
						   				ew_car = 0;
						   			}
						   			if (msg.data == 3)			//once we get this message we know not to expect any more mqueue data
						   			{
						   				printf("Got x\n");
						   				printf("\nServer received Destroy command\n");
						   				// destroyed channel before exiting
						   				ChannelDestroy(chid);
						   				pthread_exit( EXIT_FAILURE );
						   				shortcut = 0;
						   			}



						   sleep(1); // Delay the reply by a second (just for demonstration purposes)

					   printf("\n    -----> replying with: '%s'\n",replymsg.buf);
					   MsgReply(rcvid, EOK, &replymsg, sizeof(replymsg));
				   }
				   else
				   {
					   printf("\nERROR: Server received something, but could not handle it correctly\n");
				   }
	return msg.data;

}

void DeleteFile(string hostname, string filename)
{
	string dir[50]="/net/";
	String dir2[50]="/tmp/file/";
	strcat(dir,hostname);
	strcat(dir,dir2);
	strcat(dir,filename);
	int remove( const char * filename );
    if( remove( filename ) ) {
        printf( "Error removing %s",filename );

    }
}


#endif /* SERVERCOMS_H_ */
