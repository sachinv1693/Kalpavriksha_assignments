#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* A structure to hold individual Stack nodes */
typedef struct node
{
    char ch;//Holds a character
    struct node* link;//Link between every stack node
}Stack_t;

/* Function to push value into stack */
void push(Stack_t** top, char ch)
{
    //Create a new node to push new value
    Stack_t* new_node = (Stack_t*)malloc(sizeof(Stack_t));
    //Check if memory is dynamically allocated for new node
    if (new_node == NULL)
    {
        //printf("Unable to allocate dynamic memory!\n\n");
        return;
    }
    //Assign obtained value
    new_node -> ch = ch;
    //Assign new node's link with current top node
    new_node -> link = *top;
    //Update top node value
    *top = new_node;
}

/* Function to extract the top node value */
char pop(Stack_t** top)
{
    //Check if stack is empty
    if (*top == NULL)
    {
        //printf("Stack is empty!\n\n");
        return '\0';
    }
    //Store the top node address into a Stack pointer
    Stack_t* to_free = *top;
    //Get the top node value
    char ch = (*top) -> ch;
    //Update top node value
    *top = (*top) -> link;
    //Free up the memory
    free(to_free);
    return ch;
}

bool isMatchingChar(char popped, char scanned)
{
    return ((popped == '(' && scanned == ')') || (popped == '[' && scanned == ']') || (popped == '{' && scanned == '}'));
}

char* readline();

// Complete the isBalanced function below.

// Please either make the string static or allocate on the heap. For example,
// static char str[] = "hello world";
// return str;
//
// OR
//
// char* str = "hello world";
// return str;
//
char* isBalanced(char* s)
{
    static char str[4];//Define static string to return proper content
    strcpy(str, "NO");//Initialize sting to "NO"
    //Error checking as per given requirement
    if (strlen(s) % 2 || strlen(s) > 1000)
        return str;
    //Define a top node value as NULL
    Stack_t* top = NULL;
    //Scan the string till we find NUL character
    while (*s)
    {
        //If opening braces are found, push into stack
        if (*s == '(' || *s == '[' || *s == '{')
            push(&top, *s);
        //Check for repective closing braces and pop one by one item
        else if (*s == ')' || *s == ']' || *s == '}')
        {
            if (top == NULL || !isMatchingChar(pop(&top), *s))
                return str;
        }
        else//Some other character than braces, return str i.e. "NO"
            return str;
        s++;//Increment pointer
    }
    //After scanning the entire string, if balanced braces are found then stack must be empty
    //So, Return "YES"
    if (top == NULL)
    {
        strcpy(str, "YES");
        return str;
    }
    //Otherwise return "NO"
    return str;
}

int main()
{
    /*
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* t_endptr;
    char* t_str = readline();
    int t = strtol(t_str, &t_endptr, 10);

    if (t_endptr == t_str || *t_endptr != '\0') { exit(EXIT_FAILURE); }
    */

    char str[1001] = {'\0'}; 
    
    fscanf(stdin, "%s", str);
    int t = atoi(str);
    
    //fprintf(stdout, "No. of inputs: %d\n", t);
    
    for (int t_itr = 0; t_itr < t; t_itr++) {
        //char* s = readline();
        fscanf(stdin, "%s", str);
        //fprintf(stdout, "%s\n", str);
        char* result = isBalanced(str);

        fprintf(stdout, "%s\n", result);
    }

    //fclose(fptr);

    return 0;
}

/*
char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) { break; }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') { break; }

        size_t new_length = alloc_length << 1;
        data = realloc(data, new_length);

        if (!data) { break; }

        alloc_length = new_length;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
    }

    data = realloc(data, data_length);

    return data;
}
*/