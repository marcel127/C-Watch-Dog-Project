/*==============================================================================
Data Structures - double Linked List
Header
OL66
Version 1
==============================================================================*/

#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */

#include "dllist.h"

#define ITER_TO_NODE(x) ((dnode_t *)(x).info)

static void ConnectNodes(dnode_t *pre, dnode_t *new, dnode_t *post);

struct dnode
{
    void *data;
    dnode_t *next;
    dnode_t *prev;
};

struct dlist_t
{
    dnode_t first;
    dnode_t last;
};

/*---------------------------------------------------------------------------*/
/* Iterator functions: */

diter_t DLBegin(const dlist_t *list)
{
	diter_t iter = {0};
	assert(list);
	iter.info = list->first.next;
	return iter;
}

diter_t DLEnd(const dlist_t *list)
{
	diter_t iter = {0};
	assert(list);
	iter.info = (dnode_t *)&list->last;
	return iter;
}

diter_t DLNext(diter_t iter)
{
	dnode_t *node = NULL;
	
	assert(iter.info);
	
	node = iter.info;
	node = node->next; 
	iter.info = node;

	return iter;
}

diter_t DLPrev(diter_t iter)
{
	dnode_t *node = NULL;
	
	assert(iter.info);
	
	node = iter.info;
	node = node->prev; 
	iter.info = node;
	
	return iter;
}

int DLIsSameIter(const diter_t iter1, const diter_t iter2)
{
	assert(iter1.info);
	assert(iter2.info);

	return (iter1.info == iter2.info);
}

void *DLGetData(diter_t iter)
{
	dnode_t *node = NULL;
	
	assert(iter.info);
	
	node = iter.info;
	
	return node->data;
}

/*---------------------------------------------------------------------------*/
/* DList functions: */


dlist_t *DLCreate()
{
	dlist_t *list = (dlist_t *)malloc(sizeof(dlist_t));
	
	if (NULL == list)
	{
		return NULL;
	}
	
	list->first.prev = NULL;
	list->last.next = NULL;
	list->first.data = NULL;
	list->last.data = NULL;
	list->first.next = &list->last;
	list->last.prev = &list->first;

	return list;
}

void DLDestroy(dlist_t *list)
{
	assert(list);
	
	while(!DLIsEmpty(list))
	{
		DLErase(DLBegin(list));
	}
	
	free(list);
	list = NULL;
}

size_t DLCount(const dlist_t *list)
{
	diter_t iter = {0};
	size_t count = 0;
	assert(list);

	for(iter = DLBegin(list); !DLIsSameIter(iter,DLEnd(list)) ; iter = DLNext(iter))
	{
		++count;
	}  

	return count;
}

int DLIsEmpty(const dlist_t *list)
{
	assert(list);
	return (DLIsSameIter(DLBegin(list),DLEnd(list)));
}

diter_t DLInsert(dlist_t *list, diter_t iter, const void *data)
{
	assert(list);
	return DLInsertAfter(list, DLPrev(iter), data);	
}

diter_t DLInsertAfter(dlist_t *list, diter_t iter, const void *data)
{
	diter_t new = {0};
	dnode_t *node =NULL;
	
	assert(list);

	node =(dnode_t*)malloc(sizeof(dnode_t));	

	if (NULL == node)
	{
		return DLEnd(list);
	}
	
	node->data =(void *) data;
	
	ConnectNodes(ITER_TO_NODE(iter), node, ITER_TO_NODE(iter)->next);
	
	new.info = node;
	
	return new;
}

void DLErase(diter_t iter)
{
	dnode_t *pre = NULL;
	dnode_t *post = NULL;

	assert(iter.info);	

	pre = ITER_TO_NODE(iter)->prev;
	post = ITER_TO_NODE(iter)->next;	

	free(ITER_TO_NODE(iter));

	pre->next = post;
	post->prev = pre;
}

diter_t DLPushFront(dlist_t *list, const void *data)
{
	assert(list);
	return DLInsert(list, DLBegin(list), data);
}

diter_t DLPushBack(dlist_t *list, const void *data)
{
	assert(list);
	return DLInsert(list, DLEnd(list), data);
}

void DLPopFront(dlist_t *list)
{
	assert(list);
	DLErase(DLBegin(list));
}

void DLPopBack(dlist_t *list)
{	
	assert(list);
	DLErase(DLPrev(DLEnd(list)));
}

int DLForEach
(
     diter_t from,
     diter_t to,
     int (*operation_func)(void *data, void *argument),
     void *argument
)
{
	int res = 0;

	assert(from.info);
	assert(to.info);

	while(!DLIsSameIter(from,to) && 0 == res)
	{
		res = operation_func(ITER_TO_NODE(from)->data,argument);
		from = DLNext(from);
	}
	
	return res;
}


diter_t DLFind
(
 	diter_t from,
	diter_t to,
 	int(*cmp_func)(const void *data, const void *to_compare),
 	const void *to_compare
)
{
	int res = 0;

	assert(from.info);
	assert(to.info);
	
	while(!DLIsSameIter(from,to))
	{
		res = cmp_func(ITER_TO_NODE(from)->data,to_compare);
		
		if (1 == res)
		{
			return from;	
		}
		
		from = DLNext(from);
	}
	
	return to;
}

void DLSplice(diter_t from, diter_t to, diter_t dest)
{
	diter_t prev_to = {0};

	assert(from.info);
	assert(to.info);
	assert(dest.info);

	prev_to = DLPrev(to);

	ITER_TO_NODE(prev_to)->next->prev = ITER_TO_NODE(from)->prev;
	ITER_TO_NODE(from)->prev->next = ITER_TO_NODE(prev_to)->next;
	
	ITER_TO_NODE(from)->prev = ITER_TO_NODE(dest)->prev;
	ITER_TO_NODE(dest)->prev->next = ITER_TO_NODE(from);
	
	ITER_TO_NODE(dest)->prev = ITER_TO_NODE(prev_to);
	ITER_TO_NODE(prev_to)->next = ITER_TO_NODE(dest);

}
/****************************************************************
HELPER FUNCTION
***************************************************************/
static void ConnectNodes(dnode_t *pre, dnode_t *new, dnode_t *post)
{
	new->next = post;
	new->prev = pre;
	pre->next = new;
	post->prev = new;
}

