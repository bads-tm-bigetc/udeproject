#ifndef TREAP_H_
#define TREAP_H_


typedef int (*OrdFunc) (void* ,void* );
typedef struct _Tree Tree;
typedef struct _TreapNode TreapNode;
typedef TreapNode* PTreapNode;


struct _Tree
{
  PTreapNode top;
  OrdFunc compare;
};

struct _TreapNode
{
  void* key;
  void* data;
  unsigned prior;
  PTreapNode left, right;
};

/*
 * Initialise tree with default values
 */
void tree_init(Tree* tr, OrdFunc cmp);

/*
 * Split tree tr into two trees (lft and rgt) in the following way:
 * All nodes with key less than or equal to "key" will go to lft,
 * other to rgt. lft and rgt must point to initialised trees
 */
void tree_split(OrdFunc cmp, PTreapNode tr, PTreapNode* lp, PTreapNode* rp, void* key);

/*
 * Merge trees lft and rgt into one new tree. All keys in lft must be
 * strictly less than in rgt.
 */
PTreapNode tree_merge(PTreapNode lp, PTreapNode rp);

/*
 * Insert new node into tr with "key" and "data. If there already exists
 * a node with such key, than function return the original data and
 * overwrite it with new. Otherwise NULL is returned;
 */
void* tree_insert(Tree *tr, void* key, void* data);

/*
 * Find node with "key" and return pointer to it. If no node found
 * NULL is returned;
 */
PTreapNode tree_find_node(Tree *tr, void* key);

/*
 * Find node with "key" and return pointer to its data. If no node found
 * NULL is returned;
 */
void* tree_find(Tree *tr, void* key);

/*
 * Find node with "key" and erase it. Its data and key are returned
 * to old_key and old_data. If no node is erased, old_key and old_data = NULL
 */
void tree_erase(Tree *tr, void* key, void** old_key, void** old_data);

/*
 * Traverses tree tr from smaller nodes to larger and calls for
 * them function f
 */
void tree_for_each(Tree *tr, void (*f)(PTreapNode cur) );

/*
 * Traverse left subtree of cur, then apply f to cur, then traverse
 * right subtree of cur
 */
void tree_traverse(PTreapNode cur, void (*f)(PTreapNode cur) );

#endif
