//===========HEADER=============
#define _DEFAULT_SOURCE //To avoid usleep warning
#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
void child();
void parent();  
void process_stage(int stage);
void test_child_pipe(int *pipe_fd);
void test_parent_pipe(int *pipe_fd);

//Represents an array of file descriptor read/write end of pipes.
int *pipe_fd;
int stages=0;
//===========HEADER=============

//WRITE YOUR CODE HERE

//Create a pipe_fd using the pipe_fd system call and return the 
//file descriptor array.
int *create_pipe()
{
    int* fd = (int*) malloc(2 * sizeof(int));
    pipe(fd);
	return fd;
}


//Write a message to the pipe_fd
size_t send_message(int pipe_fd_fd, char * message)
{
       size_t write_size = write(pipe_fd_fd, message, strlen(message));
       return write_size;
}
size_t read_message(int pipe_fd_fd, char *out_message,int length)
{
       size_t read_size = read(pipe_fd_fd, out_message, length + 1);
       return read_size;
}
//Setup child, close the appropriate file descriptor pipe_fd[0] or pipe_fd[1] 
//depending on whether messages will be sent from child to parent vs parent to child.
void setup_child(int *pipe_fd)
{
	//Write your code here.
    close(pipe_fd[0]);
}

//finalize child, close the appropriate file descriptor pipe_fd[0] or pipe_fd[1] 
//depending on which one is open.
void finalize_child(int *pipe_fd)
{
	//Write your code here.
    close(pipe_fd[1]);
}

//Setup parent, close the appropriate file descriptor pipe_fd[0] or pipe_fd[1] 
//depending on whether messages will be sent from child to parent vs parent to child.
void setup_parent(int *pipe_fd)
{
	//Write your code here.
    close(pipe_fd[1]);
}

//finalize parent, close the appropriate file descriptor pipe_fd[0] or pipe_fd[1] 
//depending on which one is open.
void finalize_parent(int *pipe_fd)
{
	//Write your code here.
    close(pipe_fd[0]);
}
//Get the exit code of the child.
int get_child_exit_status()
{
        //Write your code here.
        wait(&stages);
        return (WIFEXITED(stages)) ? WEXITSTATUS(stages) : stages;
}

//DO NOT CHANGE THE CODE BELOW
//
void child(){
		
	setup_child(pipe_fd);

	for(int i=1;i<=stages;i++)	
	{
		char message[32];
		sprintf(message,"%s%d","STAGE",i);
		process_stage(i);
 		//Stage 1 done	
		send_message(pipe_fd[1],message);
		usleep(1);
		
	}

 	finalize_child(pipe_fd); 
	test_child_pipe(pipe_fd);
        exit(stages); 
}
void parent()
{
        
 	setup_parent(pipe_fd); 
	for(int i=1;i<=stages;i++)	
	{
		char message[1024]; 
		printf("Waiting for the child to finish the stage:%d\n",i);
		 fflush(stdout);
		size_t size=0;
		do
		{
			size = read_message(pipe_fd[0],message,100);
			if(size>0)
			{
				printf("STAGE completed:%s\n",message);
		
			}
			
		}while(size<=0);
		
		fflush(stdout);
	}

 	finalize_parent(pipe_fd); 
	test_parent_pipe(pipe_fd);

	printf("Child exited with status:%d\n",get_child_exit_status());
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
	pipe_fd = create_pipe(); 
	if(pipe_fd == NULL)
	{
		printf("Pipe creation failed\n");
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
void test_parent_pipe(int *pipe_fd)
{
	char message[32];
	int size=send_message(pipe_fd[1],"test");
	if(size>0)
	{
		printf("Parent:Write end of the pipe is not closed, check finalize_parent()\n");
	}
	size=read_message(pipe_fd[0],message,32);
	if(size>0)
	{
		printf("Parent:Read end of the pipe is not closed, check setup_parent()\n");
	}
}
void test_child_pipe(int *pipe_fd)
{
	char message[32];
	int size=send_message(pipe_fd[1],"test");

	if(size>0)
	{
		printf("Child:Write end of the pipe is not closed, check finalize_child()\n");
	}
	usleep(10);
	printf("Child:Testing read end of the pipe, program may hang if the read end is not closed in the child, check implementation of setup_child()\n");
	size=read_message(pipe_fd[0],message,32);
}
