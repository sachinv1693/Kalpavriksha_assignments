#include<stdio.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include <stdio_ext.h>
#include<fcntl.h>
#include "string.h"

#define OPEN_SUCCESS "File Open Successful\n"
#define OPEN_ERROR "File Open Failed\n"

void create_file();
void open_file();
size_t read_file(FILE *fp,char *buffer,int length);
void process_students();
void move_to_next_student(FILE *fp, int current);
typedef struct student_type
{
	int id;
	char name[32];
	char college[32];
	char passing_year[16];
} student_t;
int compare_student(student_t *student1, student_t * student2);

//MODIFY BELOW functions
FILE* open_file_in_read_mode(char *file_name)
{
        FILE* fp = fopen((const char*) file_name, "r");
        if (fp)
		    //Print this message if the open is successful
		    printf(OPEN_SUCCESS);
        else
  		    //Print this message when open is failed
		    printf(OPEN_ERROR);
  		
	return fp;
}
FILE* open_file_in_read_write_mode(char *file_name)
{       
        FILE* fp = fopen((const char*) file_name, "r+");
        if (fp)
		    //Print this message if the open is successful
		    printf(OPEN_SUCCESS);
  		else
            //Print this message when open is failed
		    printf(OPEN_ERROR);
  		
	return fp;
}

//Read the Student record and return the pointer to student structure
//The format of the student record is as follows
//id - integer 
//name - string
//college - string
//passing_year - string

student_t* read_student_from_file(FILE *fp)
{
	//Read the student record
    student_t* student = (student_t*) calloc(1, sizeof(student_t));
    fscanf(fp, "%d", &(student -> id));
    fseek(fp, 1L, SEEK_CUR);
    fscanf(fp, "%s", student -> name);
    fseek(fp, 1L, SEEK_CUR);
    fscanf(fp, "%s", student -> college);
    fseek(fp, 1L, SEEK_CUR);
    fscanf(fp, "%s", student -> passing_year);
    fseek(fp, 1L, SEEK_CUR);
    return student;
}
//This function should read the data at a given location in the file.
//Hint: use fseek() to go to the specific location
//position - position in the file
//length - length of data to be read
size_t read_from_location(FILE *fp, char * buffer, int position, int length)
{
    fseek(fp, position,SEEK_SET);
    size_t size = fread(buffer, 1, length, fp);
	return size;
}

//While writing Student data, use space as separator for the fields
void write_student_to_file(FILE *fp, student_t *student)
{
	//Write code to store student record in file
    fprintf(fp, "%d", student -> id);
    fprintf(fp, " ");
    fprintf(fp, "%s", student -> name);
    fprintf(fp, " ");
    fprintf(fp, "%s", student -> college);
    fprintf(fp, " ");
    fprintf(fp, "%s", student -> passing_year);
    fprintf(fp, " ");
}
//Implement the write_file function, the first parameters is the file descriptor
//returned by open function call and 2nd parameter is the data to be written.
size_t write_file(FILE * fp,char *data)
{
	//write your code here
    size_t size = fprintf(fp, "%s", data);
    fputc('\0', fp);
    return size + 1;
}
//Implement the read_file function, 
//This should read the data from the FILE pointer (fp) into the input buffer. 
//buffer - buffer where the data needs to be read.
//length - specifies the length of the data to read.
size_t read_file(FILE *fp,char *buffer,int length)
{
	//write your code here
    size_t size = fread(buffer, 1, length, fp);
    return size;
}

//==============================================
//DO NOT MODIFY CODE BELOW
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
        FILE *fp = open_file_in_read_mode("test.txt");
    if(create<=0){
        return 0;
    }    
    if(fp<=0)
        {
            printf("Invalid file descriptor returned by open_file_in_read_mode\n");
        }

    //Test the write, if the write 
    // int size=(int)write_file(fp,buffer);
    // if(size>0){
    //     printf("Invalid open mode\n");
    // }

    fclose(fp);    

    fp = open_file_in_read_write_mode("test.txt");

    //Test the write, if the write  works
    size_t write_size=(int)write_file(fp,buffer);
    if(write_size<input_data_length || write_size >(input_data_length + 1)){
        printf("Write Failed\n");
    }
    fclose(fp);
    fp = open_file_in_read_mode("test.txt");

    char buffer_read[1024];
    int size = (int)read_file(fp,buffer_read,1024);
    if(size!=write_size){
        printf("Invalid Size of the data returned. size = %d, write_size = %ld\n", size, write_size);
    }

    printf("Data Written:%s\n",buffer);
    printf("Data Read:%s\n",buffer_read);
    if(strcmp(buffer,buffer_read))
    {
        printf("DataWritten did not match with Data Read\n");
    } 
    fclose(fp);
    process_students();

}
void process_students()
{
	int student_count;
	student_t *students[100];
    	FILE *fp = open_file_in_read_write_mode("test.txt");
	scanf("%d",&student_count);
	for(int i=0;i<student_count;i++)
	{
		student_t *student = read_student_from_file(stdin);
		printf("id:%d,name:%s,college:%s,passing_year:%s\n",
			student->id,student->name,student->college,student->passing_year);
		write_student_to_file(fp,student);
		students[i] = student;
	}
	fseek(fp,0,SEEK_SET);
	for(int i=0;i<student_count;i++)
	{
		student_t *student = read_student_from_file(fp);
		if(!compare_student(student,students[i]))
		{
			printf("Read/Write Student did not work properly\n");
		}	
		
	}
  
   //Dump file data.
	fseek(fp,0,SEEK_END);
    
	int file_size=ftell(fp);
    fseek(fp,0,SEEK_SET);
    char buffer_read[2048];
    read_file(fp,buffer_read,2048);
   buffer_read[file_size]='\0';
    printf("File Data:%s\n",buffer_read);
    
	fseek(fp,0,SEEK_END);

	int position=file_size/2;
	char buffer[1024];
	size_t size=read_from_location(fp,buffer,position,position);
	buffer[size]='\0';
	printf("Data Read at location:%d, is %s and size is:%zu\n",position, buffer,size);
	
	fclose(fp);
}
int compare_student(student_t *student1, student_t * student2)
{
	if(student1->id!=student2->id)
	{
		return 1;
	}
	if(!strcmp(student1->name,student2->name))
	{
		return 1;
	}
	if(!strcmp(student1->college,student2->college))
	{
		return 1;
	}
	if(!strcmp(student1->passing_year,student2->passing_year))
	{
		return 1;
	}
	return 0;
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
