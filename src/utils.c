#include "utils.h"

static Node* heap;

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

Node* getNode(Node* head, const unsigned int n)
{
    Node* ptr = head;

    if (head == NULL) return NULL;

    for (int i = 0; i < n; i++)
    {
        if (ptr->next == NULL) return NULL;
        ptr = ptr->next;
    }

    return ptr;
}

void assert(void* ptr, const short action)
{
    if (ptr == NULL)
    {
        printf(">> Error: memory failure.\n");
        purge();
        exit(EXIT_FAILURE);
    }

    if (action == APPEND)
    {
        addNode(&heap, ptr);
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