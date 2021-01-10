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
	char data[3];
	struct node_t *next;
} node;

/* Linked List holding references to head and tail of the list*/
typedef struct linked_list_t
{
	node *head,* tail;
} linked_list;

/* Function to remove the defective gene from dna sequence */
void remove_defective_gene(linked_list** dna_sequence, node* dna_new_itr, node* new_next)
{
    node* temp;//Temporary node to hold new iterator address
    //If new iterator holds head address of dna sequence, then head is updated with new next address
    if ((*dna_sequence) -> head == dna_new_itr)
    {
        (*dna_sequence) -> head = new_next;
        temp = dna_new_itr;//temp node iterates from new iterator address
    }
    else//Otherwise temp node iterates from new iterator's next node address
        temp = dna_new_itr -> next;
    if (new_next == NULL)//If new next is NULL, it means defective sequence carries the end node of dna sequence
        (*dna_sequence) -> tail = temp;//Update tail node to temp node address
    //Iterate temp node till we reach new next node
    while (temp != new_next)
    {
        //Logic to free each node one by one
        node* to_move = temp -> next;//Hold next address in another pointer
        free(temp);//Free current node
        temp = to_move;//Advance to next address
    }
    dna_new_itr -> next  = new_next;//Assign new next address as new iterator's next node address
}

void fix_dna_sequence(linked_list* dna_sequence, linked_list *gene_sequence)
{
    //Add your code here.
    node* dna_itr = dna_sequence -> head;//Iterator pointer holding head node address
    
    //If any of one of the linked lists is empty, just return
    if (dna_itr == NULL || gene_sequence -> head == NULL)
        return;
    
    //Search for defective gene sequence in the dna_sequence
    node* dna_new_itr = dna_itr;
    
    while (dna_itr)//Iterate over dna sequence
    {
        //Check if data at first node of defective sequence matches with current iterator's data 
        if (!strcmp(dna_itr -> data, gene_sequence -> head -> data))
        {
            //Start matching data from remaining nodes
            node* dna_temp = dna_itr -> next;
            node* gene_temp = gene_sequence -> head -> next;
            while (dna_temp && gene_temp)//iterate both till any one reaches to NULL
            {
                if (strcmp(dna_temp -> data, gene_temp -> data))//Break the loop in case of unmatching
                    break;
                dna_temp = dna_temp -> next;
                gene_temp = gene_temp -> next;
            }
            //gene sequence reaching at NULL means we found the sequence
            //and remove the gene_sequence from the dna_sequence
            if (!gene_temp)
            {
                //Remove the found sequence, update iterator value and continue in the loop
                remove_defective_gene(&dna_sequence, dna_new_itr, dna_itr = dna_temp);
                continue;
            }
        }
        //Advance to next node addresses
        dna_new_itr = dna_itr;
        dna_itr = dna_itr -> next;
    }
}



void list_init(linked_list **list)
{
	*list = malloc(sizeof(linked_list));
	(*list)->head = NULL;
	(*list)->tail = NULL;
}

int add(linked_list *list, char *item)
{
	node *new_node= (node*)malloc(sizeof(node));	
	strcpy(new_node->data,item);
	new_node->next = NULL;

	if(list->head == NULL)
	{
		list->head=new_node;
		list->tail=new_node;
	}
	else
	{
		list->tail->next = new_node;
		list->tail = new_node;
	}

	return 0;
}


void list_display(linked_list *list)
{
	node *current=list->head;
	printf("List : ");
	while(current!=NULL)
	{
		printf("%s,",current->data);
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

int main(int argc, char * argv[])
{
    linked_list *dna_sequence;
    linked_list *gene_sequence;
    list_init(&dna_sequence);
    list_init(&gene_sequence);
  
    int l_dna_sequence= parse_int(ltrim(rtrim(readline())));
    for (int i = 0; i < l_dna_sequence; i++) {
        char * l_item = ltrim(rtrim(readline()));
        
        add(dna_sequence, l_item);
    }
    int l_gene_sequence= parse_int(ltrim(rtrim(readline())));
    for (int i = 0; i < l_gene_sequence; i++) {
        char * l_gene = ltrim(rtrim(readline()));
        
        add(gene_sequence, l_gene);
    }
    
    //printf("Input DNA Sequence ");list_display(dna_sequence);
    //printf("Input Gene Sequence ");list_display(gene_sequence);
    fix_dna_sequence(dna_sequence,gene_sequence);
    printf("Corrected DNA Sequence ");list_display(dna_sequence);
    
    return 0;
}
