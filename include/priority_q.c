#include <assert.h>		   /* assert */
#include <stdlib.h>		   /* malloc */

#include "priority_q.h"

#define NEED_TO_ERASE (1)

struct pq_s
{
	sortedlist_t *p_q;
};

pq_t *PriorityQCreate(is_prior_t is_prior)
{
	pq_t *pq =(pq_t *)malloc(sizeof(pq_t));
	
	if (NULL == pq)
	{
		return NULL;
	}

	pq->p_q = SortedListCreate(is_prior);

	if (NULL == pq->p_q)
	{
		free(pq);
		return NULL;
	} 
	
	return pq;
}

void PriorityQDestroy(pq_t *pq)
{
	assert(pq);
	
	SortedListDestroy(pq->p_q);
	free(pq);
	
	pq = NULL;	
}

int PriorityQEnqueue(pq_t *pq, void *data)
{
	sliter_t iter = {0};
	
	assert(pq);	
	
	iter = SortedListInsert(pq->p_q,data);

	return SortedListIsSameIter(iter,SortedListEnd(pq->p_q)); 
}

void PriorityQDequeue(pq_t *pq)
{
	assert(pq);	

	SortedListPopBack(pq->p_q);	
}

size_t PriorityQSize(const pq_t *pq)
{
	assert(pq);	

	return SortedListCount(pq->p_q);
}

int PriorityQIsEmpty(const pq_t *pq)
{
	assert(pq);	

	return SortedListIsEmpty(pq->p_q);
}

void *PriorityQPeek(const pq_t *pq)
{
	sliter_t iter = {0};
	
	assert(pq);
	
	iter = SortedListPrev(SortedListEnd(pq->p_q));

	return SortedListGetData(iter);
}

void PriorityQClear(pq_t *pq)
{
	sliter_t start = {0};	
	
	assert(pq);

	start = SortedListBegin(pq->p_q);
	
	while (!SortedListIsSameIter(start,SortedListEnd(pq->p_q)))
	{
		PriorityQDequeue(pq);
		start = SortedListBegin(pq->p_q);
	}
}

void PriorityQErase(pq_t *pq, criteria_func_t criteria_func, void *arg)
{
	sliter_t iter_begin = {0};
	sliter_t iter_end = {0};
	sliter_t runner = {0};
	int res = 0;

	assert(pq);

	iter_begin = SortedListBegin(pq->p_q);
	iter_end = SortedListEnd(pq->p_q);

	while(!SortedListIsSameIter(iter_begin,iter_end))
	{
		runner = iter_begin;
		iter_begin = SortedListNext(iter_begin);
		res = criteria_func(SortedListGetData(runner),arg);
		
		if (NEED_TO_ERASE == res)
		{
			SortedListErase(runner);
		}
	}

}

