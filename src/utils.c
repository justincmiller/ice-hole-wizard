/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "utils.h"

//head of a linked-list holding nodes to the pointers to malloc'd pointers
static Node* heap;

//VT100 Input initialization
bool virtualInput()
{
    HANDLE input = GetStdHandle(STD_INPUT_HANDLE);
    if (input == INVALID_HANDLE_VALUE)
    {
        printf("Error: could not fetch input handle\n");
        return false;
    }
    
    DWORD mode;
    if (!(GetConsoleMode(input, &mode)))
    {
        printf(">> Error: could not fetch input mode\n");
        return false;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_INPUT;
    if (!(SetConsoleMode(input, mode)))
    {
        printf(">> Error: could not set input mode\n");
        return false;
    }

    return true;
}

//VT100 Output initialization
bool virtualOutput()
{
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); //handle to the console's standard output
    if (output == INVALID_HANDLE_VALUE)
    {
        printf("Error: could not fetch output handle\n");
        return false;
    }

    DWORD mode;
    if (!(GetConsoleMode(output, &mode)))
    {
        printf(">> Error: could not fetch input mode\n");
        return false;
    }

    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    if (!(SetConsoleMode(output, mode)))
    {
        printf(">> Error: could not set input mode\n");
        return false;
    }

    return true;
}

//function to add node to linked-list
void addNode(Node** head, void* data)
{
    Node* node = malloc(sizeof(Node));
    ASSERT(node);
    node->data = data;
    node->next = NULL;
    node->prev = NULL;
    
    if (*head == NULL) //if list is empty
    {
        *head = node; //add node to list
    }
    else
    {
        //traverse to end of list and inserts node
        Node* ptr = *head;
        while (ptr->next != NULL)
        {
            ptr = ptr->next;
        }
        ptr->next = node;
        node->prev = ptr;
    }
}

//updates linked-list heap
void track(void* ptr)
{
    addNode(&heap, ptr); //adds a node to the "heap"
}

//free memory from malloc'd linked-lists
void purge()
{
    if (heap == NULL) return; //if no memory is allocated

    Node* ptr = heap;
    while (ptr != NULL) //if memory is allocated
    {
        Node* next = ptr->next;
        free(ptr->data);
        free(ptr);
        ptr = next;
    }

    heap = NULL; //no memory left to purge
}
