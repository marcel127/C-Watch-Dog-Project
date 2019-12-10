#ifndef PRIORITY_Q
#define PRIORITY_Q

#include <stddef.h>        /* size_t */
#include "sorted_ll.h"

typedef struct pq_s pq_t;

/*
Function will return true
if data1 is prior to data2
false otherwise 
*/
typedef int (*is_prior_t)(const void *data1, const void *data2);

/*
Function will return true
if data fits the given argument,
false otherwise 

        arguments:
                data - element data
                arg - data for comparison 
                
*/
typedef int (*criteria_func_t)(const void *data, void *arg); 

/*
        Create a new priority queue.
        
        Arguments:
                is_prior - function to prioritise by.
                
        returns a reference to the new queue.
        
        Complexity O(1)
*/
pq_t *PriorityQCreate(is_prior_t is_prior);

/*
        Destroy a given priority queue.
        
        Arguments:
                pq - the queue to destroy.
                        
        Complexity O(n)
*/
void PriorityQDestroy(pq_t *pq);

/*
        Insert a new element into the queue.
        
        Arguments:
                pq - the queue to insert to.
                data - the data to insert.
                
        returns 0 on sucsses, 
                appropriate error code on failure.
        
        Complexity O(n)
*/
int PriorityQEnqueue(pq_t *pq, void *data);

/*
        Remove the element first in line in a given queue.
        
        Arguments:
                pq - the queue to insert to.
                
        Complexity O(1)
*/
void PriorityQDequeue(pq_t *pq);

/*
        Count the number of elements in a given queue.
        
        Arguments:
                pq - the queue.
                
                
        returns number of elements.
        
        Complexity O(n)
*/
size_t PriorityQSize(const pq_t *pq);

/*
        Check if a given queue is empty.
        
        Arguments:
                pq - the queue.
                
                
        returns true is the queue is empty,
                false otherwise,
        
        Complexity O(1)
*/
int PriorityQIsEmpty(const pq_t *pq);

/*
        Return the data of the first element in a given queue
                                                                     (first in line). 
        
        Arguments:
                pq - the queue.
                
        For an empty queue, returns NULL.

        Complexity O(1)
*/
void *PriorityQPeek(const pq_t *pq);

/*
        Clear all elements in a given queue, 
                                        the queue will remain valid.
        
        Arguments:
                pq - the queue.
                        
        Complexity O(n)
*/
void PriorityQClear(pq_t *pq);

/*
        Erase all elements of a specific criteria from a given queue
        
        Arguments:
                pq - the queue.
                criteria_func - function to locate the elements to erase.
                criteria - to argument for criteria_func.        
                arg - another argument.

        Complexity O(n)
*/
void PriorityQErase(pq_t *pq, 
                                        criteria_func_t criteria_func,
                                        void *arg);

#endif /* PRIORITY_Q */
