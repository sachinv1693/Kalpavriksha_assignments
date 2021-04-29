#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<string.h> 
#include<sys/wait.h> 
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include<sys/msg.h>
#include <ctype.h>
void child();
void parent();  
int get_child_exit_status();
int msgid;
int commands=0;

typedef struct command_type
{
	long type;
	//The command to execute
	char command[32];
	//The arguments for the command
	int args[2];
} command_t;
typedef struct result_type 
{
	long type;
	int result;
} result_t;

//WRITE YOUR CODE HERE
//Create a message queue and return the message queue id
int create_message_queue()
{
    key_t key = ftok("prog_file", 65);
	return msgget(key, 0666 | IPC_CREAT);
}


//send a command via message queue
//msgid - ID of the Message queue
int send_command(int msgid, command_t cmd)
{
    return msgsnd(msgid, &cmd, sizeof(cmd), 0);    
}
//Read the command from the message queue
//msgid - ID of the Message queue
command_t *recv_command(int msgid)
{
    command_t* cmd = (command_t*) malloc(sizeof(command_t)); 
    msgrcv(msgid, cmd, sizeof(*cmd), 1, 0);
	return cmd;
}

//Send the result via message queue
//msgid - ID of the Message queue
int send_result(int msgid, result_t result)
{
    return msgsnd(msgid, &result, sizeof(result), 0);
}
//Read the Result from the message queue
//msgid - ID of the Message queue
result_t *recv_result(int msgid)
{
    result_t* result = (result_t*) malloc(sizeof(result_t));
    if (msgrcv(msgid, result, sizeof(*result), 2, 0) == -1)
        perror("recv error");
    return result;
}
//Delete the message queue
void delete_message_queue(int msgid)
{
	msgctl(msgid, IPC_RMID, NULL);
}
void parent(){

	for(int i=1;i<=commands;i++)	
	{
		command_t *cmd = (command_t*) malloc(sizeof(command_t));
        cmd -> type = 1;
      	result_t *result;
        
      	//WRITE CODE HERE TO READ THE COMMAND FROM INPUT
        fscanf(stdin, "%s", cmd -> command);
        fseek(stdin, sizeof(char), SEEK_CUR);
        fscanf(stdin, "%d", &(cmd -> args[0]));
        fseek(stdin, sizeof(char), SEEK_CUR);
        fscanf(stdin, "%d", &(cmd -> args[1]));
        
		//SEND THE COMMAND via send_command
      	send_command(msgid, *cmd);
        
        //RECIEVE THE RESULT from CHILD
        result = recv_result(msgid);
      
		printf("CMD=%s, RES = %d\n",cmd->command,result->result);
		
	}
	printf("Child exited with status:%d\n",get_child_exit_status());
	delete_message_queue(msgid);

}

int process_command(char* command, int* args)
{
    if (!strcmp(command, "ADD"))
        return args[0] + args[1];
    else if (!strcmp(command, "SUB"))
        return args[0] - args[1];
    else if (!strcmp(command, "MUL"))
        return args[0] * args[1];
    else if (!strcmp(command, "DIV") && args[1])
        return args[0] / args[1];
    else
    {
        printf("Invalid operation or command!\n");
        return -1;
    }
}

void child()
{
	for(int i=1;i<=commands;i++)	
	{
		command_t *cmd;
        //WRITE CODE to RECIEVE THE COMMAND,use recv_command method.
        cmd = recv_command(msgid);
        result_t result;
        result.type = 2;
      	//WRITE CODE to process the command.
        result.result = process_command(cmd -> command, cmd -> args);
        //SEND RESULT via send_result
        send_result(msgid, result);
	}

        exit(commands); 
}

//DO NOT MODIFY CODE BELOW
int main(int argc, char* argv[]) 
{ 
	pid_t cid; 
	msgid = create_message_queue(); 
	scanf("%d",&commands);
	if(msgid <= 0 )
	{
		printf("Message Queue creation failed\n");
	}
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

//Get the exit code of the child.
int get_child_exit_status()
{
        int stat;
        wait(&stat);
        return WEXITSTATUS(stat);
}