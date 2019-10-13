#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <mqueue.h>
#include <errno.h>
#include <sys/iofunc.h>
//written functions
//#include <ClientComs.h>
//#include <ServerComs.h>

#define BUF_SIZE 100
#define Buffer_size 1

#define B                   66
#define Q                   81
#define D                   68
#define N                   78
#define O                   79
#define A                   65
#define T                   84
#define F                   70
#define S                   83
#define E                   69
#define W                   87
#define R                   82
#define Y                   89
#define G                   71
#define L                   76
#define P                   80


char message[256]={""};
int messageready = 0;

typedef struct
{
	struct _pulse hdr; // Our real data comes after this header
	int ClientID; // our data (unique id from client)
    char data[5];     // our data
} my_data;

typedef struct
{
	struct _pulse hdr;  // Our real data comes after this header
    char buf[BUF_SIZE]; // Message we send back to clients to tell them the messages was processed correctly.
} my_reply;




typedef struct
{
	char buffer[Buffer_size]; // Buffer
	int readfinnished; // Used to tell producer that consumer is finished reading
	pthread_mutex_t mutex;  // needs to be set to PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t condvar; // needs to be set to PTHREAD_COND_INITIALIZER;
}app_data;

void printhelp()
{
	printf("This is a help menu\n");
	printf("To select an option just put in the number\n");
	printf("Use this command to do this\n");
	printf("Use this command to do this\n");
	printf("Use this command to do this\n");
	printf("Use this command to do this\n");
	printf("Use this command to do this\n");
	printf("Use this command to do this\n");
	printf("Use this command to do this\n");
	printf("Use this command to do this\n");
	printf("Use this command to do this\n");
}

void message_from_x1()
{
	//this will be change later to thread
	//using mqueue or something to get the state and store in variable
	//psudo code

/*	if(strcmp(message_from_x1,"3T")==0)//Error
	{

	}
	else if(strcmp(message_from_x1,"3F")==0)//Error
	{

	}
	else if(strcmp(message_from_x1,"TET")==0)
	{

	}
	else if(strcmp(message_from_x1,"TET")==0)
	{

	}
	else if(strcmp(message_from_x1,"TWT")==0)
	{

	}
	else if(strcmp(message_from_x1,"3T")==0)
	{

	}
	else if(strcmp(message_from_x1,"3F")==0)
	{

	}*/
	//
}



/*** Server code ***/
void *server(void *Not_used)
{
	int serverPID=0, chid=0; 	// Server PID and channel ID
	int num;
	FILE *fptr;
	my_data msg;
	my_reply reply;

	serverPID = getpid(); 		// get server process ID

	// Create Channel
	chid = ChannelCreate(_NTO_CHF_DISCONNECT);
	if (chid == -1)  // _NTO_CHF_DISCONNECT flag used to allow detach
	{
	    printf("\nFailed to create communication channel on server\n");
	    pthread_exit( EXIT_FAILURE );
	}

	printf("Server Listening for Clients on:\n");
	printf("These printf statements can be removed when the myServer.info file is implemented\n");
	printf("  --> Process ID   : %d \n", serverPID);
	printf("  --> Channel ID   : %d \n\n", chid);

	/*
	 *   Your code here to write this information to a file at a known location so a client can grab it...
	 *
	 *   The data should be written to a file like:
	 *   /tmp/myServer.info
	 *	 serverPID  (first line of file)
 	 *	 Channel ID (second line of file)
	 */

	fptr = fopen("/tmp/file/file.info","w");

	if(fptr == NULL)
	{
		printf("Error!\nPlease check file directory");
		exit(1);
	 }

	fprintf(fptr,"%d\n",serverPID);
	fprintf(fptr,"%d",chid);
	fclose(fptr);

		// We would have pre-defined data to stuff here
	    msg.hdr.type = 0x00;
	    msg.hdr.subtype = 0x00;
	    char buf[BUF_SIZE] = {};
	    while(1)
	    {
	    	while(messageready == 1)
			{
				strcpy(msg.data,message);
				if (MsgSend(chid, &msg, sizeof(msg), &reply, sizeof(reply)) == -1)
				{
					printf(" Error data '%s' NOT sent to server\n", msg.data);
					// maybe we did not get a reply from the server
					break;
				}
				else
				{ // now process the reply
					printf("   -->Reply is: '%s'\n", reply.buf);
				}
				messageready = 0;
			}
	    }


	    // Close the connection wont execute unless launch a new thread for every message
	   // printf("\n Sending message to server to tell it to close the connection\n");
	    //ConnectDetach(server_coid);
	    pthread_exit( EXIT_FAILURE );
}






void *UserInterface(void *Not_used)
{
	//sleep(3);
	printf(".......Please enter login details.......\n");
				char username[] = "";
				char correctuser[]="admin";
				char passwordd[]="";
				char correctpass[]="password";
				int tries=0;
				int logged_in=0;
				int left=0;

				while(1)
				{
					printf("\nUser name: ");
					fflush(stdout);
					scanf("%s",&username);
					int ret = strcmp(username,correctuser);
					if(ret <= 0) //wtf???????? password works but == doent work for this
					{
						break;
					}
					else
					{
						printf("Incorrect user name, please try again");
					}
				}

				while(1)
				{
					printf("Please Enter Password here: ");
					fflush(stdout);
					if(tries>=10)
					{
						printf("Too many log in attempts, Portal is now locked!\n");
						//if you can use timers run a timer then exit to put in username again
						return 0;
					}
					else;

					scanf("%s",&passwordd);
					int ret=strcmp(passwordd,correctpass);
					//printf("debug: ret= %d\n",ret);
					if(ret == 0)
					{
						printf("User logged in\n");
						logged_in = 1;
						break;
					}
					else
					{
						tries++;
						left=10-tries;
						printf("Password incorrect, you have %d tries left\n",left);

					}
				}

				while(logged_in == 1)
				{
					char add[]="";
					int option1;


					printf("Main Menu select one\n");
					printf("--> 1.Talk to train intersection\n");
					printf("--> 2.Talk to traffic light Controller\n");
					printf("--> 3.Logout\n");
					printf("--> 4.help\n");
					scanf("%d",&option1);//this is where niks code would go
					switch(option1)
					{
						case 1:
							printf("-->1.Control Boom gates\n");
							printf("-->2.Train red light East\n");
							printf("-->3.Train red light West\n");
							//printf("-->3.Change to night time operation mode\n");
							//printf("-->4.Change to day time operation mode\n");
							printf("-->5.Go back to main menu\n");
							int option1_1;
							int option1_2;

							scanf("%d",&option1_1);
							switch(option1_1)
							{
							case 1:

								printf("1. Close boom gate\n");
								printf("2. Open boom gate\n");
								fflush(stdout);
								scanf("%d",&option1_2);
								if(option1_2 == 1)
								{
									printf("Closing boom gate\n");
									strcpy(message, "3BF"); //mutex lock this
									messageready = 1; //mutex this which will be read by another thread to send the message
									printf("Sending message");
									break;
								}
								else if(option1_2 == 2)
								{
									printf("Opening boom gate\n");
									strcpy(message, "3BT"); //mutex lock this
									messageready = 1; //mutex this which will be read by another thread to send the message
									break;
								}
								else
								{
									printf("There is no third option");
									break;
								}
							case 2:
								printf("1. True\n"); //need to change wording
								printf("2. False\n"); //need to change wording
								printf("3. Auto\n"); //need to change wording
								fflush(stdout);
								scanf("%d",&option1_2);
								if(option1_2 == 1)
								{
									printf("Traffic light East true\n");
									strcpy(message, "ET"); //mutex lock this
									messageready = 1; //mutex this which will be read by another thread to send the message
									printf("Sending message");
									break;
								}
								else if(option1_2 == 2)
								{
									printf("Traffic light East false\n");
									strcpy(message, "EF"); //mutex lock this
									messageready = 1; //mutex this which will be read by another thread to send the message
									break;
								}
								else if(option1_2 == 3)
								{
									printf("Traffic light East auto\n");
									strcpy(message, "EA"); //mutex lock this
									messageready = 1; //mutex this which will be read by another thread to send the message
									break;
								}
								else
								{
									printf("There is no fourth option");
									break;
								}
								break;
							case 3:
								printf("1. True\n"); //need to change wording
								printf("2. False\n"); //need to change wording
								printf("3. Auto\n"); //need to change wording
								fflush(stdout);
								scanf("%d",&option1_2);
								if(option1_2 == 1)
								{
									printf("Traffic light West true\n");
									strcpy(message, "WT"); //mutex lock this
									messageready = 1; //mutex this which will be read by another thread to send the message
									printf("Sending message");
									break;
								}
								else if(option1_2 == 2)
								{
									printf("Traffic light West false\n");
									strcpy(message, "WF"); //mutex lock this
									messageready = 1; //mutex this which will be read by another thread to send the message
									break;
								}
								else if(option1_2 == 3)
								{
									printf("Traffic light East auto\n");
									strcpy(message, "WA"); //mutex lock this
									messageready = 1; //mutex this which will be read by another thread to send the message
									break;
								}
								else
								{
									printf("There is no fourth option");
									break;
								}
								break;
							/*case 3:
								printf("Changing to night time operation\n");
								strcpy(message, "nighttime"); //mutex lock this
								messageready = 1; //mutex this which will be read by another thread to send the message
								break;
							case 4:
								printf("Changing to day time operation\n");
								strcpy(message, "daytime"); //mutex lock this
								messageready = 1; //mutex this which will be read by another thread to send the message
								break;*/
							case 5:
								printf("Going back to Main menu\n");
								break;
							default:
								printf("Invalid option choose again\n");
							}
						case 2:
							//char add[]="";
							//strcat(message,add);
							break;
						case 3:
							return 0;
							break;
						case 4:
							printhelp();
							break;
						default:
							printf("Invalid input ");
							break;
					}
			}
			//TC to X1 messages
				int options5_1;
				switch (options5_1)
				{
				case 1:
					printf("")
				}
}




int main(int argc, char *argv[])
{
	printf("Welcome to Train Controller Portal\n");
	pthread_t  Login;
	void *retval;
	pthread_attr_t Login_attr;
	struct sched_param Login_param;
	// Initialise thread attribute object to the default values (required)
	pthread_attr_init (&Login_attr);
	// Explicitly set the scheduling policy to round robin
	pthread_attr_setschedpolicy (&Login_attr, SCHED_RR);
	// Set thread priority (can be from 1..63 in QNX, default is 10)
	Login_param.sched_priority = 5;
	pthread_attr_setschedparam (&Login_attr, &Login_param);
	// Now set attribute to use the explicit scheduling settings
	pthread_attr_setinheritsched (&Login_attr, PTHREAD_EXPLICIT_SCHED);
	// Increase the thread stacksize
	pthread_attr_setstacksize (&Login_attr, 8000);


	pthread_t  th2;
	//void *retval;

	while(1)
	{
		pthread_create(&Login,&Login_attr, UserInterface, NULL);
		pthread_create (&th2, NULL, server, NULL);


		pthread_join(Login,&retval);
		//pthread_join(th2,&retval);
		printf("\nLogged out :)");
	}



	printf("\nMain Terminating....");
	return EXIT_SUCCESS;
}
