#include <stdlib.h>		/* malloc */
#include <assert.h>		/* assert */
#include <time.h>

#include "ilrd_uid.h"   /* ilrd_uid_t */
#include "priority_q.h" /*priority_q_t*/
#include "scheduler.h"	/*sch_t*/

#define CONTINUE_RUN (0)

/*********************************************************************
					Task Functions
*********************************************************************/
typedef struct task_s task_t;

static task_t *TaskCreate(size_t interval,opt_t op,void *arg);

static void TaskDestroy(task_t *task);

static int TaskStart(task_t *task);

static void TaskUpdate(task_t *task);

/*********************************************************************
					Helper Functions
*********************************************************************/
static int IsBefore(const void *data, const void *to_compare);

static int EraseOp(const void *data, void *arg);

static void SleepCheck(time_t time_to_sleep);
/*********************************************************************
					Task Struct and Functions
*********************************************************************/
struct task_s
{
	size_t interval;
	time_t time_to_run;
	ilrd_uid_t uid;
	opt_t op;
	void *arg;
};

static task_t *TaskCreate(size_t interval,opt_t op,void *arg)
{
	task_t *task = (task_t *)malloc(sizeof(task_t));
	
	if (NULL == task)
	{
		return NULL;
	}
	
	task->interval = interval;
	task->time_to_run =time(NULL) + interval;
	task->op = op;
	task->arg = arg;
	task->uid = UIDGet();	
	
	if (UIDIsBad(task->uid))
	{
		TaskDestroy(task);
		
		return NULL;
	}
		
	return task;
}

static void TaskDestroy(task_t *task)
{
	assert(task);
	
	free(task);
	task = NULL;
}

static int TaskStart(task_t *task)
{
	assert(task);
	
	return task->op(task->arg);
}

static void TaskUpdate(task_t *task)
{	
	assert(task);
	
	task->time_to_run = time(NULL) + task->interval;
}

/*********************************************************************
					Scheduler Struct and Functions
*********************************************************************/
struct sch_s
{
	pq_t *sch;
};

sch_t *SchCreate(void)
{
	sch_t *sch = (sch_t *)malloc(sizeof(sch_t));

	if (NULL == sch)
	{
		
		return NULL;
	}

	sch->sch = PriorityQCreate(IsBefore);

	if (NULL == sch->sch)
	{
		free(sch);
		
		return NULL;
	}
	
	return sch;
}

void SchDestroy(sch_t *sch)
{
	assert(sch);
	
	SchStop(sch);
	PriorityQDestroy(sch->sch);
	
	free(sch);
	sch = NULL;
}

size_t SchSize(const sch_t *sch)
{
	assert(sch);
	
	return PriorityQSize(sch->sch);
}

int SchIsEmpty(const sch_t *sch)
{
	assert(sch);
	
	return PriorityQIsEmpty(sch->sch);
}

ilrd_uid_t SchAdd(sch_t *sch, size_t interval, opt_t operation, void *arg)
{
	task_t *task = NULL;
	
	assert(sch);
	
	task = TaskCreate(interval, operation, arg);
	
	if (NULL == task)
	{
		return UIDGetBad();
	}
	
	PriorityQEnqueue(sch->sch, task);
	
	return task->uid;
}

void SchRemove(sch_t *sch, ilrd_uid_t uid)
{
	assert(sch);
	
	PriorityQErase(sch->sch, EraseOp, &uid);
}

ilrd_uid_t SchRun(sch_t *sch)
{
	int operation_res = 0;
	ilrd_uid_t uid = {0};
	
	assert(sch);
	
	while (!SchIsEmpty(sch))
	{
		task_t *task = PriorityQPeek(sch->sch);
		time_t current_time = time(NULL);
		
		PriorityQDequeue(sch->sch);
		uid = task->uid;

		if(task->time_to_run >= current_time)
		{
			SleepCheck(task->time_to_run - current_time);
		}
		
		operation_res = TaskStart(task);
		
		if (CONTINUE_RUN == operation_res)
		{			
			TaskUpdate(task);
			PriorityQEnqueue(sch->sch, task);
		}
		else
		{
			
			TaskDestroy(task);
		}
	
	}
	
	return uid;
}

void SchStop(sch_t *sch)
{
	assert(sch);
	
	while (!SchIsEmpty(sch))
	{
		task_t *task = PriorityQPeek(sch->sch);
		PriorityQDequeue(sch->sch);
		TaskDestroy(task);
	}
	
}

/*********************************************************************
					Helper Functions
*********************************************************************/

static int EraseOp(const void *data, void *arg)
{
	ilrd_uid_t remove_uid = *(ilrd_uid_t *)arg;
	task_t *task = (task_t *)data;
	
	if (UIDIsSame(remove_uid, task->uid))
	{
		TaskDestroy(task);
		return 1;
	}
	
	return 0;
}

static void SleepCheck(time_t time_to_sleep)
{
    while (0 != time_to_sleep)
    {
        time_to_sleep = sleep(time_to_sleep);
    }
}


/*method to sort the P_Q*/
static int IsBefore(const void *data, const void *to_compare)
{
	const task_t *task_data = data;
	const task_t *task_to_compare = to_compare;	
	
	return ((task_data->time_to_run < task_to_compare->time_to_run) ? 0 : 1);
}


