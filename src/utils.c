/*
* utils.c
*
* Contains
*/

/*
*  Credit is given to Dr. Larry Hughes for providing the reference code
*  that was used both as inspiration and used as is throughout this program.
*/

#include "utils.h" //VT100 & linked-lists

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
    if (node == NULL) 
    { 
            printf(">> Error: memory failure.\n");
            purge();
            exit(EXIT_FAILURE);
    }
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

//function to remove node from linked-list
void removeNode(Node** head, Node* node)
{
    if (*head == NULL || node == NULL) return;

    if (node == *head)
    {
        *head = node->next;
        if (*head)
            (*head)->prev = NULL;
    }
    else
    {
        node->prev->next = node->next;
        if (node->next)
            node->next->prev = node->prev;
    }
}

//searches for node in linked-list
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

//checks to see if memory is allocated properly, if not free memory and exit
void assert(void* ptr, const short action)
{
    //if no memory will purge list and exit program
    if (ptr == NULL)
    {
        printf(">> Error: memory failure.\n");
        purge();
        exit(EXIT_FAILURE);
    }

    //if memory, adds to linked-list
    if (action == APPEND)
    {
        addNode(&heap, ptr);
    }
}

//removes node from linked-list and tracker
void forget(void* ptr)
{
    Node* node = heap;

    while (node != NULL)
    {
        if (node->data == ptr)
        {
            removeNode(&heap, node);
            free(node->data);
            free(node);
            return;
        }
        node = node->next;
    }
}

void track(void* ptr)
{
    addNode(&heap, ptr); //adds a node to the "heap"
}

//free memory from malloc'd linked-lists
void purge()
{
    if (heap == NULL) return; //if no memory is allocated

    Node* ptr = heap;
    Node* next = ptr->next;

    while (ptr != NULL) //if memory is allocated
    {
        free(ptr->data);
        free(ptr);

        ptr = next;
        next = ptr->next;
    }

    heap = NULL; //no memory left to purge
}
