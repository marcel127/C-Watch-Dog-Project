/*==============================================================================
Data Structures - sorted Linked List
Header
OL66
Version 2.0
==============================================================================*/
#ifndef SORTED_LL_H
#define SORTED_LL_H

#include <stddef.h> /* size_t */

typedef struct sortedlist_s sortedlist_t;

/*
struct sortedlist_s
{
    dlist_t *list;
    is_before_t is_before;
};
*/

typedef struct
{
  void *info;
} sliter_t;

typedef int (*s_operation_t)(void *data, void *arg);
typedef int (*is_before_t)(const void *data, const void *to_compare);
typedef int (*is_match_t)(const void *data, const void *to_cmp, void *arg);

/*
  Creates a new sorted list.

  Arguments:
    cmp_func - the operation perform in order to sort the list.

  Returns a pointer to the new list

  Complexity O(1)
*/
sortedlist_t *SortedListCreate(is_before_t before_func);

/*
  Destroy a given list, erase every member.

  Argument:
    list.

  Returns a pointer to the new list

  Complexity O(n)
*/
void SortedListDestroy(sortedlist_t *list);

/*
    Count number of elements in a given list.

    Argument:
     list.

    Returns the number of members in the list.

    Complexity O(n)
*/

size_t SortedListCount(const sortedlist_t *list);

/*
  Check if a given list is empty

  Argument:
    list.

  Returns true for an empty list,
          false otherwise

  Complexity O(1)
*/
int SortedListIsEmpty(const sortedlist_t *list);

/*
  Insert a new member to a given list,
      will be located accourding to it's value.

  Argument:
    list.
    data.

  Returns the iterator for the new member
    on failure, returns End.

  Complexity O(n)
*/

sliter_t SortedListInsert(sortedlist_t *list, void *data);

/*
  Erase a given iterator.

  Argument:
    iter - element to erase.

    Returns:
            data of the node.
            if failed return End

  Complexity O(1)
*/

void *SortedListPopBack(sortedlist_t *list);

/*
  Erase a given iterator(the first element in a given list).

  Argument:
      list.
      Returns:
              data of the node,
              if failed return End
  Complexity O(1)
*/

void *SortedListPopFront(sortedlist_t *list);

/*
  Perform a given operation between two given iterators,
    using a given argument.

  Argument:
      from - first element
      to - element to finish operating (not included)
      opt_func - the operation to perform
      argument - to pass as argument to the operation

  Returns 0 on success, appropriate error code on failure

  Complexity O(n)
*/

int SortedListForEach
              (
                sliter_t from,
                sliter_t to,
                s_operation_t opt_func,
                void *arg
              );

/*
  Search for an element between two given iterators.

  Argument:
      from - first element to be searched
      to - element to finish searching (not included)
      data - data to search for

  Returns iterator to the found member,
    Returns End (the argument) if member was not found.

  Complexity O(n)
*/

sliter_t SortedListFind(
                        sliter_t from,
                        sliter_t to,
                        is_match_t is_match,
                        const void *to_find,  void *arg
                     );
/*
    Merge two given lists.

    Argument:
       dest - where new list will start
       src - elements from this list will be added to dest

    Complexity O(n)
*/

void SortedListMerge(sortedlist_t *dest, sortedlist_t *src);

/******************************iter functions**********************************/

/*
        Returns an iterator to first element in the list.

        Arguments:
                list.

        complexity O(1)
*/
sliter_t SortedListBegin(sortedlist_t *list);

/*
  Returns the last element in a given list (not a valid location).

  Argument:
      list.

  Complexity O(1)
*/

sliter_t SortedListEnd(sortedlist_t *list);

/*
  Returns the next element after a given iterator.

  Argument:
      iterator.

  Complexity O(1)
*/

sliter_t SortedListNext(sliter_t iter);

/*
  Returns data contained in a given iterator

  Argument:
      iterator.

  Complexity O(1)
*/

sliter_t SortedListPrev(sliter_t iter);

/*
  Check if 2 given iterators a equal.

  Argument:
      iterator1.
      iterator2.

  Returns true if equal, false otherwise

  Complexity O(1)
*/

sliter_t SortedListErase(sliter_t iter);

/*
  Erase the last element in a given list.

  Argument:
      list.

  Complexity O(1)
*/

int SortedListIsSameIter(sliter_t iter1, sliter_t iter2);

/*
  Returns the first element in a given list.

  Argument:
      list.

  Complexity O(1)
*/

void *SortedListGetData(sliter_t iter);

#endif /* SORTED_LL_H */
