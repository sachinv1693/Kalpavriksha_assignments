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

struct thread_node {
  char name[10];
  int priority;
  struct thread_node *next;
};
typedef struct thread_node thread_node;

struct priority_queue {
  thread_node *front, *rear;
};
typedef struct priority_queue priority_queue;

bool frontmostOrRearmostNode(priority_queue* queue, thread_node* thread);
void makeNewFrontNode(priority_queue* queue, thread_node* thread);
void makeNewRearNode(priority_queue* queue, thread_node* thread);
void adjustThreadNodes(priority_queue* queue, thread_node* thread, thread_node* node);

bool frontmostOrRearmostNode(priority_queue* queue, thread_node* thread)
{
    if (!(queue -> front) && !(queue -> rear))//If no node is present in queue, update queue's front and rear pointers
    {
        queue -> front = queue -> rear = thread;
        return true;
    }
    if (thread -> priority < queue -> front -> priority)//Check if it is the highest priority thread
    {
        makeNewFrontNode(queue, thread);
        return true;
    }
    if (thread -> priority >= queue -> rear -> priority)//Check if it is the lowest priority thread
    {
        makeNewRearNode(queue, thread);
        return true;
    }
    return false;
}

void makeNewFrontNode(priority_queue* queue, thread_node* thread)
{
    thread -> next = queue -> front;
    queue -> front = thread;
}

void makeNewRearNode(priority_queue* queue, thread_node* thread)
{
    queue -> rear -> next = thread;
    queue -> rear = thread;
}

void adjustThreadNodes(priority_queue* queue, thread_node* thread, thread_node* node)
{
    thread -> next = node -> next;
    node -> next = thread;
}

void enqueue(priority_queue *queue, thread_node *thread) {
  //Add your code in this method. Enqueue Thread into queue.
    if (frontmostOrRearmostNode(queue, thread))//Check for frontmost or rearmost thread node, adjust and return
        return;
    thread_node* current, *previous;//current and previous node for iterating through the queue
    current = previous = queue -> front;//Initialize both with front node
    while (current)//Iterate till end of queue
    {
        if (thread -> priority < current -> priority)//Attach before current node if higher priority thread
        {
            adjustThreadNodes(queue, thread, previous);
            return;
        }
        else if (thread -> priority == current -> priority && current -> next -> priority != thread -> priority)//Check for equal priority threads
        {
                adjustThreadNodes(queue, thread, current);//Adjust nodes if next node also has the same priority
                return;
        }
        previous = current, current = current -> next;//Update previous and current node addresses
    }
}

thread_node* dequeue(priority_queue *queue) {
  //Add your code in this method to Dequeue Thread from queue as per priority order.
  //Return the order of execution.
    thread_node* to_be_returned = queue -> front;//front node address to be returned
    queue -> front = queue -> front -> next;//Update front pointer
    if (!(queue -> front))//Make rear pointer equal to NULL upon reaching to all nodes in the queue
        queue -> rear = NULL;
    return to_be_returned;//Return obtained address
}


/*=======DO NOT MODIFY THE CODE BELOW =======*/

thread_node* create_node(char *thread_name, int thread_priority) {
  thread_node *thread;
  thread = malloc(sizeof(thread_node));
  thread->priority = thread_priority;
  strcpy (thread->name, thread_name);
  thread->next = NULL;

  return thread;
}

//Get new queue
priority_queue* create_queue() {
  priority_queue *queue = malloc(sizeof(priority_queue));
  queue->front = NULL;
  queue->rear = NULL;

  return queue;
}

void display(thread_node *thread) {
        printf("%s %d", thread->name, thread->priority);
        printf("\n");
}

int main(int argc, char * argv[])
{
        int thread_count, thread_priority;
        char thread_name[10];

        priority_queue *queue = create_queue();

        scanf("%d", &thread_count);

        thread_node *thread[thread_count];

        //Input the Thread priority table and put it in queue.
        for (int index = 0; index < thread_count; index++) {
        scanf("%s %d",thread_name,&thread_priority);

        thread[index] = create_node(thread_name, thread_priority);

        enqueue(queue, thread[index]);
        }

        for (int index = 0; index < thread_count; index++) {
                //dequeue will return threads
                display(dequeue(queue));
       }
}