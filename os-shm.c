#define _DEFAULT_SOURCE //To avoid usleep warning
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
void child();
void parent();  
void process_stage(int stage);
//Represents an array of file descriptor read/write end of pipes.
int shmid;
int stages=0;

//--WRITE YOUR CODE BELOW--

//Create a Shared Memory segment and return the SHMID
int create_shared_mem(int size)
{
 	return shmget('B', size, IPC_CREAT | 0664);
}


//Write a message to the shared memory.
void write_message(int shmid, char * message)
{
	char* shm_ptr = (char*) shmat(shmid, NULL, 0);
    if (shm_ptr)
    {
        strcpy(shm_ptr, (const char*) message);
        shmdt(shm_ptr);
    }
}
//Read the message from the shared memory and return the data.
//shmid - ID of the shared memory segment.
//length - Length of data to read.
char *read_message(int shmid, int length)
{
    char* read_message = (char*) malloc(sizeof(char) * length);
    char* shm_ptr = (char*) shmat(shmid, NULL, 0);
    if (shm_ptr)
    {
        strcpy(read_message, (const char*) shm_ptr);
        shmdt(shm_ptr);
    }
	return read_message;
}
void remove_shared_mem(int shmid)
{    
    shmctl(shmid, IPC_RMID, NULL);
}

//-----DO NOT MODIFY THE CODE BELOW-----

//Get the exit code of the child.
int get_child_exit_status()
{
        int stat;
        wait(&stat);
        return WEXITSTATUS(stat);
}
void child(){
		

	for(int i=1;i<=stages;i++)	
	{
		char message[32];
		sprintf(message,"%s%d","STAGE",i);
		process_stage(i);
 		//Stage 1 done	
		write_message(shmid,message);
		usleep(10000);
		
	}

        exit(stages); 
}
void parent()
{
	char *last_message=NULL;
	char *message=NULL; 

	for(int i=1;i<=stages;i++)	
	{
		printf("Waiting for the child to finish the stage:%d\n",i);
		fflush(stdout);
		//usleep(1000);
		//The following loop is a hack to ensure parent waits for the
   		//new message from child in a loop. In future we will use
		//Semaphore to ensure synchronization.
		do
		{

			usleep(1000);

			last_message=message;
			message=read_message(shmid,32);	

		}while((last_message==NULL && message == NULL) 
		|| (last_message!=NULL && strcmp(last_message,message)==0));

		printf("STAGE completed:%s\n",message);


		fflush(stdout);
	}

	printf("Child exited with status:%d\n",get_child_exit_status());
	remove_shared_mem(shmid);
}

void process_stage(int stage)
{
	//
	//Some complex logic is executed here, we just do a printf for now
	//
	printf("Procesing stage%d\n",stage);
	usleep(1);
	fflush(stdout);
}

int main(int argc, char* argv[]) 
{ 
	pid_t cid; 
	shmid = create_shared_mem(100); 
	if(shmid == 0 )
	{
		printf("Shared Mem creation failed\n");
	}
	scanf("%d",&stages);
	cid = fork(); 

	// Parent process 
	if (cid == 0) 
	{ 
		child();
	} else if(cid > 0 )
	{
		parent();
	}
}