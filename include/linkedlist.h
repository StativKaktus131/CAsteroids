#pragma once

#include <stdlib.h>
#include <stdio.h>

// LinkedList is basically just a node
#define LinkedList node_t

// Node with value and next pointer
typedef struct Node {
	void* value;
	struct Node* next;
	struct Node* last;
} node_t;


// ======= CREATION =======

// creates a list / node with initial value
LinkedList* ll_create(void* value) 
{
	node_t* node = (node_t *) malloc(sizeof(node_t));
	node->value = value;
	node->next = NULL;
	node->last = NULL;

	return node;
}

// creates an empty linked list (value = NULL)
LinkedList* ll_create_empty()
{
	return ll_create(NULL);
}



// ======= UTILIZE =======


// finds the list head and returns a pointer to it
LinkedList* ll_head(LinkedList* list)
{
	node_t* node = list;

	// iterate through list
	while (node->next)
		node = node->next;

	return node;
}

// returns the size of the linked list
int ll_size(LinkedList* list)
{
	node_t* node = list;
	int i = 0;
	
	if (!node->value)
		return 0;

	for (;node; i++)
		node = node->next;
	
	return i;
}

// iterates through the list and performs the action with the index and value
void ll_for_each(LinkedList* list, void (*action)(int, void*))
{
	node_t* node = list;

	if (!node->value)
		return;

	for (int i = 0; node; i++)
	{
		action(i, node->value);
		node = node->next;
	}
}

// adds an element with given value at the front of the list
void ll_push_front(LinkedList** list, void* value) 
{
	// if the first value is null (list is empty) create a node at the first point
	if (!(*list)->value)
	{
		*list = ll_create(value);
		return;
	}

	// create a new node and set its next node to the first list element, update 'last' pointer too
	node_t* node = ll_create(value);
	node->next = *list;
	(*list)->last = node;

	// shift the list pointer
	list = &node;
}

void ll_push_back(LinkedList** list, void* value)
{
	// if the first value is null (list is empty) create a node at the first point
	if (!(*list)->value)
	{
		*list = ll_create(value);
		return;
	}

	node_t* head = ll_head(*list);
	node_t* new_node = ll_create(value);
	
	head->next = new_node;
	new_node->last = head;
}

void ll_pop_front(LinkedList** list)
{
	printf("POP FRONT\n");

	node_t* node = *list;

	// if there are any next projectiles, shift list
	if (node->next)
	{
		node_t* next = node->next;
		next->last = NULL;
		free(node); // WARNING: implement custom free behaviour
		*list = next;
	}
	else
	{
		// one head should always be not null, just set value to null
		node->value = NULL;
	}
}

void ll_pop_back(LinkedList** list)
{
	node_t* head = ll_head(*list);
	head->last->next = NULL;
	free(head);
}

void ll_remove_at(LinkedList** list, int index)
{
	// find node at index
	node_t* node = *list;

	for (int i = 0; i < index; i++)
		node = node->next;
	
	// utilize doubly linked list to swap ends
	if (node->next && node->last)
	{
		node_t* last = node->last;
		node->last->next = node->next;
		node->next->last = last;
	}

	else if (index == 0)
	{
		ll_pop_front(list);
	}

	else if (index == ll_size(*list)-1)
	{
		ll_pop_back(list);
	}
}


// ======= INSPECTION =======

int ll_indexof(LinkedList* list, void* element)
{
	node_t* node = list;

	// if list is empty, return nothing
	if (!list->value) 
		return -1;

	// loop through list
	for (int i = 0; node; i++)
	{
		// pointer equals pointer
		if (node->value == element)
			return i;

		// step through list
		node = node->next;
	}

	// -1 as error value
	return -1;
}

void* ll_get(LinkedList* list, int index)
{
	node_t* node = list;

	for (int i = 0; i < index; i++)
		node = node->next;
	
	return node->value;
}



// ======= DEBUG =======

void ll_print(LinkedList* list, void (*print_function)(int, void*))
{
	node_t* node = list;

	// if list is empty, return
	if (!node)
		return;

	// iterate through list, print with given function
	for (int i = 0; node; i++)
	{
		print_function(i, node->value);
		node = node->next;
	}
}



// ======= FREE FUNCTIONALITY =======

// iterate through the list and free all nodes. to free the values as well, use free_deep
void ll_free(LinkedList* list) 
{
	node_t* node = list;
	
	while (node)
	{
		node_t* next = node->next;
		free(node);
		node = next;
	}
}

// frees the value as well
void ll_free_deep(LinkedList* list) 
{
	node_t* node = list;
	
	while (node)
	{
		node_t* next = node->next;
		free(node->value);
		free(node);
		node = next;
	}
}

// if void pointer refers to a complex struct with dynamically allocated pointers, use a custom freeing functionality
void ll_free_custom(LinkedList* list, void (*free_function)(void*))
{
	node_t* node = list;
	
	while (node)
	{
		node_t* next = node->next;
		free_function(node->value);
		free(node);
		node = next;
	}
}