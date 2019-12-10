#ifndef SH_SCHEDULER_H
#define SH_SCHEDULER_H

#include <stddef.h>          /* size_t */
#include "ilrd_uid.h"        /* ilrd_uid_t */

typedef struct sch_s sch_t;
/*
    return signals to scheduler - 0 to keep running, !0 to end
*/
typedef int (*opt_t)(void *arg);

/*
    Create a new Scheduler, returns a reference to it.
                                      NULL on failure.
*/
sch_t *SchCreate(void);

/*
    Destroy a given Scheduler.
        All tasks will me erased, all references will become invalid.

        Arguments: 
            scheduler.

    complexity O(n)
*/
void SchDestroy(sch_t *sch);

/*
    Returns number of tasks in a given scheduler.

        Arguments:
            scheduler.
*/
size_t SchSize(const sch_t *sch);

/*
    Checks if a given scheduler is empty.

        Arguments:
            scheduler.

    Returns true if empty,
            false otherwise
*/
int SchIsEmpty(const sch_t *sch);

/*
    Add a new task to a given scheduler.

        Arguments:
            scheduler. - the scheduler
                        interval - milliseconds between calls to opt
            operation - task function. Must conform with opt_t as described
                above
            arg - any other things needed for user to perform operation        

    Returns unique id of new task, bad uid otherwise
*/
ilrd_uid_t SchAdd(sch_t *sch, size_t interval, opt_t operation, void *arg);

/*
    Removes a specific task from scheduler

    Arguments:
        sch - sheduler to remove from
        uid  - id to identify task by
*/
void SchRemove(sch_t *sch, ilrd_uid_t uid);

/*
    Start scheduler

    Blcoks thread indifferently, or until a task op causes it to stop

    Arguments:
        sch - scheduler

   Returns uid of the last task performed
*/
ilrd_uid_t SchRun(sch_t *sch);

/*
    Stop run of given scheduler
    Removes all tasks, handle remains valid

    Arguments:
        sch - scheduler

*/
void SchStop(sch_t *sch);


#endif /* SH_SCHEDULER_H */
