/***  TREES - c-Module for handling binary tree structures
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

/* this implements a very simple binary tree for use with uwm's keyboard
   (and eventually other) configuration. it's mainly optimized on small
   data structure creation code, little data structure memory overhead and
   fast lookup. it is not adequate for treatment of huge trees or other
   things it has not been designed for. */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include "tree.h"

Tree *TreeRotL(Tree *tree)
{
  if(tree->less) {
    Tree *s;
    s = tree->less;
    tree->less = s->greater;
    s->greater = tree;
    return s;
  } else return tree;
}

Tree *TreeRotR(Tree *tree)
{
  if(tree->greater) {
    Tree *s;
    s = tree->greater;
    tree->greater = s->less;
    s->less = tree;
    return s;
  } else return tree;
}

Tree *DoTreeEquilibrate(Tree *t, int i)
{
  int k, l;

  k = abs(i)/2;
  l = k - 1 + (abs(i) % 2);

  if(i > 1) {
    int j;
    for(j = 0; j < k; j++) t = TreeRotL(t);
  } else if(i < -1) {
    int j;
    for(j = 0; j < l; j++) t = TreeRotR(t);
  } else {
    return t;
  }
  t->greater = DoTreeEquilibrate(t->greater, -k);
  t->less = DoTreeEquilibrate(t->less, l);
  return t;
}

TreePot *CreateTree(NodeList *list, gelfunc order)
{
  int i;
  Node *n;
  TreePot *tree;

  if(!(tree = malloc(sizeof(TreePot)))) {
    return NULL;
  }

  tree->order = order;
  tree->root = NULL;

  SortNodeList(list, order);

  n = NULL;
  i = 0;
  while(n = NodeNext(list, n)) {  /* transform NodeList into linear tree */
    Tree *t;
    if(!(t = malloc(sizeof(Tree)))) {
      while(tree->root) {
        Tree *o;
	o = tree->root->less;
	free(tree->root);
	tree->root = o;
      }
      free(tree);
      return NULL;
    }
    t->less = tree->root;
    t->greater = NULL;
    t->data = n->data;
    i++;
    tree->root = t;
  }
  tree->root = DoTreeEquilibrate(tree->root, i);
  return tree;
}

Tree *GetFromTree(TreePot *tree, void *key)
{
  Tree *t;
  int gel;

  t = tree->root;
  while(t && (gel = tree->order(t->data, key))) {
    t = (gel > 0) ? t->less : t->greater;
  }
  return t;
}
