#include "list.h"
#include "stdio.h"
#include "stdlib.h"

plist list_allocate()
{
	plist res=(plist) malloc(sizeof(list));
	if(res==NULL)
	{
		fprintf(stderr,"Error: out of memory\n");
		exit(1);
	}
	return res;
}

pnode list_allocate_node()
{
	pnode res=(pnode) malloc(sizeof(node));
	if(res==NULL)
	{
		fprintf(stderr,"Error: out of memory\n");
		exit(1);
	}
	res->data=NULL;
	res->next=res->prev=NULL;
	res->dummy=False;
	return res;
}

void list_init(plist l)
{
	l->count=0;
	l->head=list_allocate_node();
	l->tail=list_allocate_node();
	l->head->next = l->tail;
	l->tail->prev = l->head;
	l->head->dummy = l->tail->dummy = True;
}

void list_free(plist l)
{
	pnode t1,t2;
	t1=l->head;
	while(t1 != NULL)
	{
		t2=t1;
		t1=t1->next;
		if((t2->dummy==False)&&(t2->data!=NULL))
			free(t2);
	}
	l->count=0;
	l->head=l->tail=NULL;
}

void list_append(plist l,void *data)
{
	pnode t;
	t=list_allocate_node();
	t->data=data;
	t->prev = l->tail->prev;
	t->next = l->tail;
	t->prev->next = t;
	t->next->prev = t;
	l->count++;
}

void* list_get_nth_data(plist l,int n)
{
	pnode cur;
	cur=l->head->next;
	while((n>0)&&(cur->dummy==False))
	{
		cur=cur->next;
		n--;
	}
	if(cur->dummy==False)
		return cur->data;
	else
		return NULL;
}

void* list_get_last_data(plist l)
{
	pnode cur=l->tail->prev;
	if(cur->dummy==False)
		return cur->data;
	else
		return NULL;
}

void list_remove_last(plist l)
{
	pnode cur;
	cur=l->tail->prev;
	if(!cur->dummy)
	{
		l->count--;
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		free(cur);
	}
}

void  list_remove_nth(plist l, int n)
{
	pnode cur;
	cur=l->head->next;
	while((n>0)&&(cur->dummy==False))
	{
		cur=cur->next;
		n--;
	}
	if(cur->dummy==False)
	{
		l->count--;
		cur->prev->next = cur->next;
		cur->next->prev = cur->prev;
		free(cur);
	}
}

void list_insert_before_nth(plist l,int n,void*data)
{
	pnode t,cur;
	l->count++;
	t=list_allocate_node();
	t->data=data;
	t->dummy=False;
	cur=l->head->next;
	while((n>0)&&(cur->dummy==False))
	{
		cur=cur->next;
		n--;
	}
	if(cur->dummy)
	{
		list_append(l,data);
	}
	else
	{
		t->next=cur;
		t->prev=cur->prev;
		cur->prev->next=t;
		cur->prev=t;
	}
}
