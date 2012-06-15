#include "tree.h"
#include <stdlib.h>
#include <stdio.h>

void tree_init(Tree* tr, OrdFunc cmp)
{
  tr->top=NULL;
  tr->compare=cmp;
}

void tree_split(OrdFunc cmp, PTreapNode tr, PTreapNode* lp, PTreapNode* rp, void* key)
{
  PTreapNode cp;
  cp=tr;

  while( cp != NULL )
    {
      if( cmp(key,cp->key) >=0 )
	{
	  *lp=cp;
	  lp= &(cp->right);
	  cp=cp->right;
	}
      else
	{
	  *rp=cp;
	  rp= &(cp->left);
	  cp=cp->left;
	}
    }
  (*lp)=NULL;
  (*rp)=NULL;
}

PTreapNode tree_merge(PTreapNode lp, PTreapNode rp)
{
  PTreapNode res;
  
  PTreapNode *cp;
  cp = &( res );
    
  while ( 1 )
    {
      if( lp == NULL )
	{
	  *cp = rp;
	  break;
	}
      else if( rp == NULL )
	{
	  *cp = lp;
	  break;
	}
      else
	{
	  if( lp -> prior < rp -> prior )
	    {
	      *cp = rp;
	      cp = &( rp->left );
	      rp = rp->left;
	    }
	  else
	    {
	      *cp = lp;
	      cp = &( lp->right );
	      lp = lp->right;
	    }
	}
    }
  return res;
}

void *tree_insert( Tree *tree, void* key, void* data)
{
  PTreapNode *cur,n;
  OrdFunc cmp=tree->compare;
  
  n = tree_find_node(tree, key);
  if( n != NULL )
    {
      void* res = n->data ;
      n->data = data;
      return res;
    }

  cur = &( tree->top );
  n=(PTreapNode) malloc(sizeof(TreapNode) );
  if(n==NULL)
    {
      fprintf(stderr,"Impossible to create node: Out of memory!\n");
      exit(1);
    }
  n->key = key;
  n->data = data;
  n->prior = rand();
  n->left = NULL;
  n->right = NULL;
  while ( 1 )
    {
      if( ! (*cur) )
	{
	  (*cur)=n;
	  break;
	}
      else if( (*cur) -> prior < n -> prior )
	{
	  tree_split( cmp, (*cur), &( n->left ), &( n->right ), n->key );
	  (*cur) = n;
	  break;
    	}
      else if( cmp(key, (*cur)->key ) < 0 )
	{
	  cur = &( (*cur)->left );
	}
      else
	{
	  cur = &( (*cur)->right );
	}
    }
  return NULL;
}

PTreapNode tree_find_node( Tree* tr, void * key )
{
  PTreapNode cur = tr->top;
  OrdFunc cmp = tr->compare;
  int val;
  while ( cur != NULL )
    {
      val = cmp ( key, cur -> key );
      if( val < 0 )
	cur = cur->left;
      else if ( val == 0 )
	break;
      else
	cur = cur->right;
    }
  return cur;
}

void* tree_find(Tree* tr, void* key)
{
  PTreapNode ptr = tree_find_node( tr, key );
  return ptr ? ptr->data: NULL;
}

void tree_erase(Tree* tr, void* key, void** old_key, void** old_data)
{
  PTreapNode *cur = &(tr->top);
  OrdFunc cmp = tr->compare;
  int val;
  *old_key = NULL;
  *old_data = NULL;

  while ( (*cur) != NULL )
    {
      val = cmp( key, (*cur) -> key );
      if( val < 0 )
	{
	  cur = & ( (*cur)->left );
	}
      else if ( val == 0 )
	{
	  PTreapNode tmp=(*cur);
	  *old_key = tmp->key;
	  *old_data = tmp->data;
	  (*cur) = tree_merge( (*cur)->left, (*cur)->right );
	  free( tmp );
	  break;
	}
      else
	cur = &( (*cur) ->right );
    }
}

void tree_for_each(Tree *tr, void (*f)(PTreapNode cur) )
{
  tree_traverse(tr->top,f);
}

void tree_traverse(PTreapNode cur, void (*f)(PTreapNode cur) )
{
  if( cur != NULL )
    {
      PTreapNode rgt = cur->right;
      tree_traverse( cur->left, f);
      f( cur );
      tree_traverse( rgt, f );
    }
}
