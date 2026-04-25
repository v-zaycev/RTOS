/*************************************/
/*             resource.c            */
/*************************************/

#include "sys.h"
#include "rtos_api.h"
#include <stdio.h>


void InitResource(TResourceId id)
{
	for (int i = 0; i < OSData.ResourcesCount; ++i)
	{
		if (OSData.Resources[i].id == id)
		{
			Log("Resource %d already Inited", message_warning, id);
			return;
		}
	}

	int free_place = OSData.ResourcesCount;

	if (free_place >= MAX_RES)
	{
		Log("Max number of Resources already Registered", message_error);
		return;
	}

	Log("InitResource %d", message_info, id);

	OSData.Resources[free_place].id = id;
	OSData.Resources[free_place].owner = -1;
	OSData.Resources[free_place].WaitingListHead = -1;

	for (int i = 0; i < MAX_TASK; ++i)
	{
		OSData.Resources[free_place].WaitingQueue[i].data = -1;
		OSData.Resources[free_place].WaitingQueue[i].next = -1;
		OSData.Resources[free_place].WaitingQueue[i].prev = -1;
	}
	++OSData.ResourcesCount;

	return;
}

void GetResource(TResourceId id)
{
	int res_pos = -1;
	for (int i = 0; i < OSData.ResourcesCount; ++i)
	{
		if (OSData.Resources[i].id == id)
		{
			res_pos = i;
			break;
		}
	}

	if (res_pos == -1)
	{
		Log("Resource %d wasn't created, task will be terminated", message_error, id);
		TerminateTask();
	}

	if (OSData.Resources[res_pos].owner == -1)
	{
		OSData.Resources[res_pos].owner = OSData.RunningTask;
		return;
	}

	AddTaskToWaitlist(OSData.RunningTask, res_pos);
	OSData.Tasks[OSData.RunningTask].status = task_waiting;
	OSData.RunningTask = -1;

	int owner = OSData.Resources[res_pos].owner;
	int owner_priority = OSData.Tasks[owner].current_priority;
	int max_priority = GetMaxPriority(owner);
	
	if (owner_priority < max_priority)
	{
		OSData.Tasks[owner].current_priority = max_priority;
		if (OSData.Tasks[owner].status == task_ready)
		{
			RemoveTaskFromQueue(owner);
			Schedule(owner);
		}
	}

	Dispatch();
}

void ReleaseResource(TResourceId id)
{
	int res_pos = -1;
	for (int i = 0; i < OSData.ResourcesCount; ++i)
	{
		if (OSData.Resources[i].id == id)
		{
			res_pos = i;
			break;
		}
	}

	if (res_pos == -1)
	{
		Log("Resource %d wasn't created, skipping release action", message_warning, id);
		return;
	}

	if (OSData.Resources[res_pos].owner != OSData.RunningTask)
	{
		Log("Task %s is not owner of Resource %d, skipping release action", message_warning,
			OSData.Tasks[OSData.RunningTask].name, id);
		return;
	}


	Log("ReleaseResource %d", message_info, id);
	
	OSData.Resources[res_pos].owner = -1;

	if (OSData.Resources[res_pos].WaitingListHead != -1)
	{
		int head = OSData.Resources[res_pos].WaitingListHead;
		int top_waiter = OSData.Resources[res_pos].WaitingQueue[head].data;
		OSData.Resources[res_pos].WaitingListHead = OSData.Resources[res_pos].WaitingQueue[head].next;
		OSData.Resources[res_pos].WaitingQueue[head].data = -1;
		OSData.Resources[res_pos].WaitingQueue[head].next = -1;
		OSData.Resources[res_pos].WaitingQueue[head].prev = -1;
		OSData.Resources[res_pos].owner = top_waiter;
		OSData.Tasks[top_waiter].status = task_ready;
		Schedule(top_waiter);
	}

	int current_priority = OSData.Tasks[OSData.RunningTask].current_priority;
	int max_priority = GetMaxPriority(OSData.RunningTask);

	if (max_priority < current_priority)
	{
		int current_task = OSData.RunningTask;
		OSData.Tasks[current_task].current_priority = max_priority;
		OSData.RunningTask = -1;
		Schedule(current_task);
	}

	Dispatch();
}
