#ifndef LIST_H_
#define LIST_H_

#include "X11/Xlib.h"
typedef struct LIST  list;
typedef struct LIST* plist;
typedef struct NODE  node;
typedef struct NODE* pnode;

struct LIST
{
	int count;
	pnode head,tail;
};

struct NODE
{
	Bool dummy;
	void *data;
	pnode next,prev;
};

/*
 * Allocates list
 */
plist list_allocate();

/*
 * Allocates and initializes new node
 */
pnode list_allocate_node();

/*
 * Initializes list setting count to 0 and
 * creating dummy head and tail
 */
void  list_init(plist l);

/*
 * Frees all nodes of the list and
 * data of all non dummy nodes
 */
void  list_free(plist l);

/*
 * Creates node with data and inserts it
 * before the tail which is a dummy node
 */
void  list_append(plist l,void* data);

/*
 * Removes node with number n. The node after
 * head has number zero
 */
void  list_remove_nth(plist l, int n);

/*
 * Gets the data from node with number n. The node after
 * head has number zero
 */
void* list_get_nth_data(plist l, int n);

/*
 * Gets the data from the node before tail.
 */
void* list_get_last_data(plist l);

/*
 * Removes the node before tail.
 */
void list_remove_last(plist l);

/*
 * Inserts node before number with number n. The node
 * after head has number zero. If n is greater than
 * number of nodes in list, data is appended
 */
void list_insert_before_nth(plist l,int n,void* data);

#endif
