#define OPEN_SUCCESS "File Open Successful\n"
#define OPEN_ERROR "File Open Failed\n"
void create_file();
void open_file();
#include<stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

//WRITE YOUR CODE HERE, INCLUDE ALL NECESSARY FILES BEFORE PROCEEDING

//Implement this function and return the file descriptor
int open_file_in_read_mode(char *file_name)
{
        int fd = open(file_name, O_RDONLY);
        if (fd > 0)
            //Print this message if the open is successful
		    printf(OPEN_SUCCESS);
        else if (fd == -1)
  		    //Print this message when open is failed
		    printf(OPEN_ERROR);
  		return fd;
 }
//Implement this function and return the file descriptor
int open_file_in_read_write_mode(char *file_name)
{
		int fd = open(file_name, O_RDWR);
        if (fd > 0)
            //Print this message if the open is successful
		    printf(OPEN_SUCCESS);
        else if (fd == -1)
            //Print this message when open is failed
		    printf(OPEN_ERROR);
  		return fd;
 
}
//Implement the write_file function, the first parameters is the file descriptor
//returned by open function call and 2nd parameter is the data to be written.
size_t write_file(int fd,char *data)
{
        size_t write_size = (size_t) write(fd, data, strlen(data) + 1);
		return write_size;
}
//Implement the read_file function, the first parameters is the file descriptor
//returned by open function call,
size_t read_file(int fd,char *buffer,int length)
{
        size_t read_size = read(fd, buffer, length);
		return read_size;
}

//DO NOT MODIFY THE CODE BELOW

int main(int argc, char *argv[])
{
	char buffer[1024];
    int input_data_length  = 0;
    int create;
    scanf("%s",buffer);
    scanf("%d",&create);
    input_data_length = strlen(buffer);
    
    if(create>0)
    {
        create_file();
    }
        int fd = open_file_in_read_mode("test.txt");
    if(create<=0){
        return 0;
    }    
    if(fd<=0)
        {
            printf("Invalid file descriptor returned by open_file_in_read_mode\n");
        }

    //Test the write, if the write 
    int size=(int)write_file(fd,buffer);
    if(size>0){
        printf("Invalid open mode\n");
    }

    close(fd);    

    fd = open_file_in_read_write_mode("test.txt");

    //Test the write, if the write  works
    size_t write_size=(int)write_file(fd,buffer);
    if(write_size<input_data_length || write_size >(input_data_length + 1)){
        printf("Write Failed\n");
    }
    close(fd);
    fd = open_file_in_read_mode("test.txt");

    char buffer_read[1024];
    size = (int)read_file(fd,buffer_read,1024);
    if(size!=write_size){
        printf("Invalid Size of the data returned\n");
    }
     //buffer_read[input_data_length]= '\0';

    printf("Data Read:%s\n",buffer_read);
    printf("Buffer:%s\n",buffer);
    if(strcmp(buffer,buffer_read))
    {
        printf("Did not recieve correct data from read\n");
    }

}
void create_file()
{
	pid_t cid = fork();
	if(cid == 0 )
	{
		char *args[]={"touch","test.txt",NULL};
		execvp(args[0],args); 
	}
	else
	{
		wait(NULL);
	}
}