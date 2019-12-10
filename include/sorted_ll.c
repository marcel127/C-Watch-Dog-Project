#include <stddef.h> /* size_t */
#include <assert.h> /* assert */
#include <stdlib.h> /* malloc */
#include <stdio.h>

#include "dllist.h"
#include "sorted_ll.h"


static sliter_t DiterToSliter(diter_t diter);
static diter_t SliterToDiter(sliter_t sliter);

struct sortedlist_s
{
	dlist_t *list;
	is_before_t is_before;
};

sortedlist_t *SortedListCreate(is_before_t before_func)
{
	sortedlist_t *sort_list = (sortedlist_t *)malloc(sizeof(sortedlist_t));
		
	if (NULL == sort_list)
	{
		return NULL;
	}
	
	sort_list->list = DLCreate();

	if (NULL == sort_list->list)
	{
		free(sort_list);		
	
		return NULL;
	}

	sort_list->is_before = before_func;

	return sort_list;
}

void SortedListDestroy(sortedlist_t *list)
{
	assert(list);
	
	DLDestroy(list->list);
	
	free(list);
}

size_t SortedListCount(const sortedlist_t *list)
{
	return DLCount(list->list);
}

int SortedListIsEmpty(const sortedlist_t *list)
{
	return DLIsEmpty(list->list);
}

sliter_t SortedListInsert(sortedlist_t *list, void *data)
{
	sliter_t iter = SortedListBegin(list);

	while (!((SortedListIsSameIter(iter,SortedListEnd(list)))) 
		  					&& !(list->is_before(data,SortedListGetData(iter))))
		{
			iter = SortedListNext(iter);
		}

	iter = DiterToSliter(DLInsert(list->list, SliterToDiter(iter), data));

	return iter;
}

void *SortedListPopBack(sortedlist_t *list)
{
	void *data = SortedListGetData(SortedListPrev(SortedListEnd(list)));
	
	DLPopBack(list->list);
	
	return data;		
}

void *SortedListPopFront(sortedlist_t *list)
{
	void *data = SortedListGetData(SortedListBegin(list));
	
	DLPopFront(list->list);
	
	return data;
}


int SortedListForEach
              (
                sliter_t from,
                sliter_t to,
                s_operation_t opt_func,
                void *arg
              )
{
	return DLForEach(SliterToDiter(from), SliterToDiter(to), opt_func, arg);
}

sliter_t SortedListFind
					 (
                        sliter_t from,
                        sliter_t to,
                        is_match_t is_match,
                        const void *to_find,  void *arg
                     )
{
	int res = 0;
	
	while(!SortedListIsSameIter(from,to))
	{
		res = is_match(SortedListGetData(from), to_find, arg);
		
		if (1 == res)
		{
			return from;	
		}
		
		from = SortedListNext(from);
	}
	
	return from;
}

sliter_t SortedListErase(sliter_t iter)
{
	sliter_t new_iter = SortedListNext(iter);
	sliter_t tmp_iter = {0};

	tmp_iter.info = iter.info;
	
	DLErase(SliterToDiter(tmp_iter)); 

	return new_iter; 
}

void SortedListMerge(sortedlist_t *dest, sortedlist_t *src)
{
	sliter_t dest_iter = {0};
	sliter_t from_iter = {0};
	sliter_t to_iter = {0};
	
	assert(dest);
	assert(dest->is_before);
	assert(src);	

	dest_iter = SortedListBegin(dest);
	from_iter = SortedListBegin(src);
	to_iter = SortedListNext(from_iter);

	while (!SortedListIsSameIter(dest_iter,SortedListEnd(dest)) 
			&& !SortedListIsSameIter(to_iter,SortedListEnd(src)))
	{

		if (dest->is_before(SortedListGetData(from_iter), SortedListGetData(dest_iter)))
		{
	
			while (dest->is_before(SortedListGetData(to_iter), SortedListGetData(dest_iter))
					&& !SortedListIsSameIter(to_iter,SortedListEnd(src)))
					{
						to_iter = SortedListNext(to_iter);
					}
	
		DLSplice(SliterToDiter(from_iter),SliterToDiter(to_iter),SliterToDiter(dest_iter));
		from_iter = to_iter;
		to_iter = SortedListNext(to_iter);
	
		}
	
		dest_iter = SortedListNext(dest_iter);
	}

	if (SortedListIsSameIter(dest_iter,SortedListEnd(dest)))
	{
		sliter_t to_iter = SortedListEnd(src);
		DLSplice(SliterToDiter(from_iter),SliterToDiter(to_iter),
											SliterToDiter(dest_iter));
	}

	else
	{
		void *data = SortedListGetData(SortedListBegin(src));
		SortedListInsert(dest, data);
		SortedListErase(SortedListBegin(src));
	}
}

/******************************iter functions**********************************/

sliter_t SortedListBegin(sortedlist_t *list)
{
	assert(list);

	return DiterToSliter(DLBegin(list->list));
}

sliter_t SortedListEnd(sortedlist_t *list)
{
	assert(list);
	
	return DiterToSliter(DLEnd(list->list));
}

sliter_t SortedListNext(sliter_t iter)
{
	return DiterToSliter(DLNext(SliterToDiter(iter)));
}

sliter_t SortedListPrev(sliter_t iter)
{
	return DiterToSliter(DLPrev(SliterToDiter(iter)));
}

int SortedListIsSameIter(sliter_t iter1, sliter_t iter2)
{
	return (iter1.info == iter2.info);
}

void *SortedListGetData(sliter_t iter)
{
	return DLGetData(SliterToDiter(iter));
}

/*************************************************************
			helper function
**************************************************************/


static diter_t SliterToDiter(sliter_t sliter)
{
    diter_t diter = {0};

    assert(sliter.info);

    diter.info = sliter.info;

    return diter;
}

static sliter_t DiterToSliter(diter_t diter)
{
    sliter_t sliter = {0};

    assert(diter.info);

    sliter.info = diter.info;

    return sliter;
}







