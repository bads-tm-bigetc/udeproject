/***  NODES - c-Module for handling dynamic lists
              1998 by Christian Ruppert             ***/

/* ########################################################################

   distributed with uwm - THE ude WINDOW MANAGER

   ########################################################################

   Copyright (c) : Christian Ruppert

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

   ######################################################################## */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <stdlib.h>

#include "nodes.h"



/*******************************************************************************
 *
 * InNodeList - searches for given data pointer in list and returns pointer to
 *              corresponding Node-structure if found else NULL.
 *
 *
 *******************************************************************************/

Node *InNodeList(NodeList *list,void *ptr)
{
  Node *InList=NULL;

  if(list){
    Node *p;
    p=list->first;
    while(p) {
      if(p->data==ptr){ 
        InList=p;
        break;
      }
      p=p->next;
    }
  }
  return(InList);
}


/*******************************************************************************
 *
 * NodeListCreate - creates new list and returns pointer to it or NULL if failed.
 *
 *******************************************************************************/

NodeList *NodeListCreate()
{
  NodeList *list;

  if(!(list=malloc(sizeof(NodeList)))) return(NULL);
  list->first=list->last=NULL;

  return(list);
}


/*******************************************************************************
 *
 * NodeAppend - Appends new element at end of list.
 *              Will return False if element is not in list after call, else
 *              True.
 *
 *******************************************************************************/

int NodeAppend(NodeList *list,void *ptr)
{
  Node *node;
  if(InNodeList(list,ptr)) return(True);

  if(!(node=malloc(sizeof(Node)))) return(False);
  node->prev=list->last;
  node->next=NULL;
  node->data=ptr;

  if(node->prev) node->prev->next=node;
  else list->first=node;

  list->last=node;
  return(True);
}


/*******************************************************************************
 *
 * NodeInsert - Inserts new element as the first element of list and resets the
 *              actual-pointer to first element.
 *              Will return False if element is not in list after call, else
 *              True.
 *
 *******************************************************************************/

int NodeInsert(NodeList *list,void *ptr)
{
  Node *node;
  if(InNodeList(list,ptr)) return(True);

  if(!(node=malloc(sizeof(Node)))) return(False);
  node->prev=NULL;
  node->next=list->first;
  node->data=ptr;

  if(node->next) node->next->prev=node;
  else list->last=node;

  list->first=list->actual=node;

  return(True);
}


/*******************************************************************************
 *
 *  NodeDelete - Deletes Element from List, Data must either be already deleted
 *               or saved.
 *               Returns pointer to next Node if existing, else pointer to
 *               previous one. Returns NULL if the list now is empty and pointer
 *               to Node itself if node is not in list.
 *
 *******************************************************************************/

Node *NodeDelete(NodeList *list,Node *node)
{
  Node *rn;
  if(!node) return(NULL);
  if(!InNodeList(list,node->data)) return(node);

  if(node->prev){
    node->prev->next=node->next;
    rn=node->prev;
  } else {
    list->first=node->next;
    rn=NULL;
  }
  if(node->next){
    node->next->prev=node->prev;
    rn=node->next;
  } else list->last=node->prev;

  free(node);
  return(rn);
}


/*******************************************************************************
 *
 * NodeListDelete - deletes complete list.
 *                  List does not have to be empty.
 *
 *******************************************************************************/

void NodeListDelete(NodeList **list)
{
  if(!(*list)) return;

  while((*list)->first) NodeDelete(*list,(*list)->first);
  free(*list);
  (*list)=NULL;
}

/*******************************************************************************
 *
 * NodeNext - returns pointer to following node.
 *
 *******************************************************************************/

Node* NodeNext(NodeList *list,Node *node)
{
  if(node) return(node->next);
  else return(list->first);
}

/*******************************************************************************
 *
 * NodePrev - returns pointer to previous node.
 *
 *******************************************************************************/

Node* NodePrev(NodeList *list,Node *node)
{
  if(node) return(node->prev);
  else return(list->last);
}

/*******************************************************************************
 *
 * Node2End - moves specified node to end of list.
 *
 *******************************************************************************/

void Node2End(NodeList *list,Node *node)
{
  if((!(node->next))||(!InNodeList(list,node->data))) return;

  if(node->prev) node->prev->next=node->next;
  else list->first=node->next;
  node->next->prev=node->prev;

  list->last->next=node;
  node->prev=list->last;
  list->last=node;
  node->next=NULL;
}


/*******************************************************************************
 *
 * Node2Start - moves specified node to beginning of list.
 *
 *******************************************************************************/

void Node2Start(NodeList *list,Node *node)
{
  if((!(node->prev))||(!InNodeList(list,node->data))) return;

  if(node->next) node->next->prev=node->prev;
  else list->last=node->prev;
  node->prev->next=node->next;

  list->first->prev=node;
  node->next=list->first;
  list->first=node;
  node->prev=NULL;
}
