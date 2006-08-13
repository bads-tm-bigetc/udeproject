/* Copyright (c) : Christian Ruppert */

/* Distributed as part of uwm, the ude window manager under the terms of
   the GNU General Public License Version 2 or later. See the file COPYING
   distributed with this file for details. */

#ifndef ARC_TREE_H
#define ARC_TREE_H

#include "nodes.h"

typedef struct _Tree Tree;

struct _Tree {
  struct _Tree *greater, *less;
  void *data;
};

typedef struct _TreePot {
  Tree *root;
  gelfunc order;
} TreePot;

/* creates balanced tree (for searching for an element qualified equal by
   order function (see nodes.h)) from unsorted NodeList, returns NULL on
   error */
TreePot *CreateTree(NodeList *list, gelfunc order);

/* searches tree for key, returns pointer to found element or NULL */
Tree *GetFromTree(TreePot *tree, void *key);

#endif
