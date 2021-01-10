#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct node
{
    int value;
    struct node* left; 
    struct node* right;
} node;

node* create_node(int value)
{
    node* new_node = malloc(sizeof(node));
    new_node -> value = value;
    new_node -> left = new_node -> right = NULL;
    return new_node;
}

node* insert(int value, node *tnode)
{
  /* add your code here */
    if (tnode == NULL)
        return create_node(value);
    if (value < tnode -> value)
            tnode -> left = insert(value, tnode -> left);
    else
        tnode -> right = insert(value, tnode -> right);
    return tnode;
}

void display(node* tnode)
{
   if(tnode==NULL)
   {
       return;
   }
   display(tnode->left);
   printf("%d ", tnode->value);
   display(tnode->right);
}

int main() {
    int i, num, value;
    node *root = NULL;
    scanf("%d", &num);
    for (i = 0; i < num; i++){
        scanf("%d",&value);
        root = insert(value, root);
    }
    display(root);
    return 0;
}