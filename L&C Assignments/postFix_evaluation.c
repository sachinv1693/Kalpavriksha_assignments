#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

/* Structure to hold stack node */
typedef struct my_stack
{
    int opnd;
    struct my_stack* next;
}Stack_t;

/* Structure to hold stack pointers */
typedef struct _sp{
    Stack_t* top;
    int count;
}Stack_pointer;

/* Function to push operands into the stack */
void push(Stack_pointer** sp, int data)
{
    //Create new stack node
    Stack_t* st = malloc(sizeof(Stack_t));
    //Update node values
    st -> opnd = data;
    st -> next = (*sp) -> top;
    //Update stack pointer
    (*sp) -> top = st;
    ++((*sp) -> count);
}

/* Function to pop operands */
int pop(Stack_pointer** sp)
{
    //If stack is NULL, return -1
    if ((*sp) -> top == NULL)
        return -1;
    //Store operand to be returned i.e. stack top value
    int ret = (*sp) -> top -> opnd;
    //Hold the stack top pointer to free memory
    Stack_t* to_free = (*sp) -> top;
    //Update stack top and stack count
    (*sp) -> top = (*sp) -> top -> next;
    --((*sp) -> count);
    //Free up memory and return the operand
    free(to_free);
    return ret;
}

int get_result(char* s, Stack_pointer* sp)
{
    if (strlen(s) > 100)
        return 0;
    //Array to read multi-difit numerical value
    char to_num[5] = {'\0'};//Number string; intialize with all NUL characters
    while (*s)//Run through every character till end of string
    {
        static int i = 0;//index value of to_num string
        //printf("ch = %c\n", *s);
        //Check if character it's a digit
        if (isdigit(*s))
        {
            //Insert digit character in to_num and increment pointer
            to_num[i++] = *s++;//Continue in the loop if digit is found again
            if (isdigit(*s))
              continue;
            else if (*s == '\0')//Return 0 if last character is digit
                return 0;
            to_num[i] = '\0';//Append NUL character in the end of to_num
            push(&sp, atoi(to_num));//Push into stack
            i = 0;//Reset index value
        }
        if (*s == '+' || *s == '-' || *s == '*' || *s == '/' || *s == '%')
        {
            //Stack must contain min 2 operands, otherwise invalid postfix expression
            if (sp -> count < 2)
                return 0;
            int opnd2 = pop(&sp);
            int opnd1 = pop(&sp);
            switch (*s)
            {
                case '+'://Addition
                    push(&sp, (opnd1 + opnd2));
                    break;
                case '-'://Subtraction
                    push(&sp, (opnd1 - opnd2));
                    break;
                case '*'://Multiplication
                    push(&sp, (opnd1 * opnd2));
                    break;
                case '/'://Division
                    if (opnd2)
                      push(&sp, (opnd1 / opnd2));
                    else
                      return 0;
                    break;
                default://Modulo
                    if (opnd2)
                      push(&sp, (opnd1 % opnd2));
                    else
                      return 0;
            }
        }
        else if (*s != ' ')//If some other character than space, return 0
            return 0;
        /*
        if (!isspace(*s))
          printf("Stack top: %d\n", sp -> top -> opnd);
        */
        s++;
    }
    if (sp -> count != 1)
        return 0;
    //printf("Stack count: %d\n", sp -> count);
    int ret = sp -> top -> opnd;//value on top of the stack to be returned
    pop(&sp);
    return ret;
}

int main()
{
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */    
    Stack_pointer sp;
    sp.top = NULL;//Initialize top of stack as NULL
    sp.count = 0;//Initialize stack count as 0
    
    char postfix[200] = {'\0'};
    
    fgets(postfix, sizeof(postfix), stdin);
    //fprintf(stdout, "%s\n", postfix);
    fprintf(stdout, "%d", get_result(postfix, &sp));
    
    return 0;
}
