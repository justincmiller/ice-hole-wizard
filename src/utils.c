#include "utils.h"

static Node* heap;

void addNode(Node** head, void* data)
{
    Node* node = malloc(sizeof(Node));
    ASSERT(node);
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    
    //if list is empty, add node to list
    if (*head == NULL)
    {
        *head = node;
    }
    else
    {
        //traverse to end of list and insert node
        Node* ptr = *head;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = node;
        node->prev = ptr;
    }
}

void track(void* ptr)
{
    addNode(&heap, ptr);
}

void purge()
{
    if (heap == NULL) return;

    Node* ptr = heap;
    while (ptr != NULL)
    {
        Node* next = ptr->next;
        free(ptr->data);
        free(ptr);
        ptr = next;
    }

    heap = NULL;
}