#ifndef ARC_NODES_H
#define ARC_NODES_H

#include <stddef.h>

#ifndef False
#define False 0
#endif
#ifndef True
#define True (-1)
#endif

typedef struct _Node Node;

struct _Node {
  struct _Node *next,*prev;
  void *data;
};

typedef struct _NodeList NodeList;

struct _NodeList {
  Node *first,*last,*actual;
};

Node* InNodeList(NodeList *list,void *ptr);
NodeList *NodeListCreate();
int NodeAppend(NodeList *list,void *ptr);
int NodeInsert(NodeList *list,void *ptr);
Node* NodeDelete(NodeList *list,Node *node);
void NodeListDelete(NodeList **list);
Node* NodeNext(NodeList *list,Node *node);
Node* NodePrev(NodeList *list,Node *node);
void Node2End(NodeList *list,Node *node);
void Node2Start(NodeList *list,Node *node);


#endif
