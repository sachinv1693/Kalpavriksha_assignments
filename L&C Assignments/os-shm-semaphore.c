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
#include <sys/sem.h> 

void child();
void parent();  
void process_stage(int stage);

int shmid;
//Semaphore that is used to indicate the data is written to shared memory
int write_semid;
//Semaphore that is used to indicate that data has been read from the shared
//memory.
int read_semid;

int stages=0;

//Write your code here 

struct sembuf sem_lock = { 0, -1, SEM_UNDO };
struct sembuf sem_unlock = {0, 1, SEM_UNDO };

//Create the semaphores and return the 0 if successful and -1 if failed.
int semaphore_create()
{
    write_semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    read_semid = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT);
    return (write_semid < 0 || read_semid < 0) ? -1 : 0;
}
//Wait for a signal from the parent that it is ready
//to read the message from shared memory.
void c_wait_for_parent_to_read()
{
    if (semop(write_semid, &sem_lock, 1) == -1)
        perror("semop");
}

//Data is written to the shared memory, signal the parent
//to read the data from shared memory.
void c_signal_parent_to_read()
{
    if(semop(read_semid, &sem_unlock, 1) == -1)
        perror("semop");
}

//Wait for the child to write data to shared memory.
void p_wait_for_child_write_data()
{
    if (semop(read_semid, &sem_lock, 1) == -1)
        perror("semop");
}

//Signal the child to write data to shared memory.
void p_signal_child_to_write_data()
{
    if (semop(write_semid, &sem_unlock, 1) == -1)
        perror("semop");
}

//Create a Shared Memory segment and return the SHMID
int create_shared_mem(int size)
{
 	int shmid;
    shmid = shmget('A', size, 0666 | IPC_CREAT);
	return shmid;
}


//Write a message to the shared memory.
void write_message(int shmid, char * message)
{
	char *shared_mem = (char *) shmat(shmid, 0, 0);
	strcpy(shared_mem,message);
	shmdt(shared_mem);
}
//Read the message from the shared memory and return the data.
//shmid - ID of the shared memory segment.
//length - Length of data to read.
char *read_message(int shmid, int length)
{
	char *message = (char*)malloc(length);

	char * shared_mem = (char *) shmat(shmid, 0, 0);
	strncpy(message,shared_mem,length);
	shmdt(shared_mem);

	return message;
}
void remove_shared_mem(int shmid)
{
	shmctl(shmid, IPC_RMID, 0);
}
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
		c_wait_for_parent_to_read();
		char message[32];
		sprintf(message,"%s%d","STAGE",i);
		process_stage(i);
 		//Stage 1 done	
		write_message(shmid,message);
		c_signal_parent_to_read();
	}

        exit(stages); 
}
void parent()
{


	for(int i=1;i<=stages;i++)	
	{
		char *message; 
		//char *last_message=NULL;

		printf("Waiting for the child to finish the stage:%d\n",i);
		fflush(stdout);

		//Signal the child to start writing data to shared memory.
		p_signal_child_to_write_data();

		//We wait for the child to write the data to shared memory.
		p_wait_for_child_write_data();

		message=read_message(shmid,32);	

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
	int status = semaphore_create();
	if(status != 0 )
	{
		printf("Semaphorecreation failed\n");
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