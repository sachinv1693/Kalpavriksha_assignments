#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* This represents a node in the list 
 * Allocate a new node as follows
 * node * new_node = malloc(sizeof(node));
 */
typedef struct node_t
{
	int data;
	struct node_t *next;
} node;

typedef struct linked_list_t
{
	node *head,* tail;
} linked_list;

node* traverseList(int index, linked_list* list)
{
    if (index == 0)
        return NULL;
    int node_count = 0;
    node* temp = list -> head;
    while (temp)
    {
        if (node_count++ == index)
            return temp;
        temp = temp -> next;
    }
    return NULL;
}

//Add your code in this method.
int add_at_index(linked_list *list, int index, int item)
{
    //Add your code here
  //list->head head of the list, list->tail is the tail of the list
  //Here's the process
  //1. Allocate a new node
  node* new_node = malloc(sizeof(node));
  new_node -> data = item;
  //2. Traverse to the node at an index-1, let's call it previous_node(Define this as a separate method)
  node* previous_node = traverseList(index-1, list);
    if (previous_node == NULL)
    {
        if (list -> head == NULL && index == 0)
        {
            list -> head = list -> tail = new_node;
            new_node -> next = NULL;
            return 0;
        }
        else if (list -> head && index == 0)
        {
            new_node -> next = list -> head;
            list -> head = new_node;
            return 0;
        }
    }
    //3. Set the new_node->next to previous_node->next
    new_node -> next = previous_node -> next;
    if (previous_node -> next == list -> tail)
        list -> tail = new_node;
  //3. Set previous_node->next to point to new_node
    previous_node -> next = new_node;
  return 0;
}


/*=======DO NOT MODIFY THE CODE BELOW =======*/
void list_init(linked_list **list)
{
        *list = malloc(sizeof(linked_list));
        (*list)->head = NULL;
        (*list)->tail = NULL;
}

void list_display(linked_list *list)
{
	node *current=list->head;
	printf("List : ");
	while(current!=NULL)
	{
		printf("%d,",current->data);
		current = current->next;
	}
	printf("\n");
}



char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    
    char* data = malloc(alloc_length);
    
    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);
        
        if (!line) {
            break;
        }
        
        data_length += strlen(cursor);
        
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }
        
        alloc_length <<= 1;
        
        data = realloc(data, alloc_length);
        
        if (!data) {
            data = '\0';
            
            break;
        }
    }
    
    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        
        data = realloc(data, data_length);
        
        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);
        
        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }
    
    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }
    
    if (!*str) {
        return str;
    }
    
    while (*str != '\0' && isspace(*str)) {
        str++;
    }
    
    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }
    
    if (!*str) {
        return str;
    }
    
    char* end = str + strlen(str) - 1;
    
    while (end >= str && isspace(*end)) {
        end--;
    }
    
    *(end + 1) = '\0';
    
    return str;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }
    
    return value;
}

long parse_long(char* str) {
    char* endptr;
    long value = strtol(str, &endptr, 10);
    
    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
 
   }
  return value;
}
int aaa(linked_list *l,int i){node* n=(node*)malloc(sizeof(node));n->data=i;n->next=NULL;if(l->head==NULL){l->head=n;l->tail=n;}else{l->tail->next=n;l->tail=n;}return 0;} 

int main(int argc, char * argv[])
{
	
	linked_list *list;
    list_init(&list);
    int l_index = parse_int(ltrim(rtrim(readline())));
	int l_item = parse_int(ltrim(rtrim(readline())));
    int l_count = parse_int(ltrim(rtrim(readline())));
    for (int i = 0; i < l_count; i++) {
        int l_item = parse_int(ltrim(rtrim(readline())));
	

        aaa(list, l_item);
    }
	add_at_index(list,l_index,l_item);
    list_display(list); 

    return 0;
}
