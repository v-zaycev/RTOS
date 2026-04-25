/******************************/
/*          os.c              */
/******************************/

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "sys.h"
#include "rtos_api.h"

int StartOS(TTaskId entry,int priority,const char* name, TMessage logLevel)
{
	OSData.RunningTask = -1;
	OSData.ResourcesCount = 0;
	OSData.TaskListHead = -1;
	OSData.stack = NULL;
	OSData.LogLevel = logLevel;

	log("StartOS!", message_info);

	for (int i = 0; i < MAX_TASK; i++)
	{
		EmptyTask(OSData.Tasks + i);
	}

	for (int i = 0; i < MAX_RES; i++)
	{
		EmptyResource(OSData.Resources + i);
	}
	for (int i = 0; i < MAX_TASK; i++)
	{
		OSData.TasksQueue[i].data = -1;
		OSData.TasksQueue[i].next = -1;
		OSData.TasksQueue[i].prev = -1;
	}

	if (setjmp(OSData.buff) == 0) {
			ActivateTask(entry, priority, name);
	}


	return 0;
}

void ShutdownOS()
{
	log("ShutdownOS!", message_info);
}

void EmptyTask(TTask* task)
{
	task->status = task_suspended;
	task->base_priority = -1;
	task->current_priority = -1;
	task->event_mask = 0;
	task->waiting_mask = 0;
	task->entry = NULL;
	task->jmp_flag = -1;
	task->waited_resource = -1;

	memset(task->stack, 0, sizeof(task->stack));
}

void EmptyResource(TResource* resource)
{
	resource->id = -1;
	resource->original_priority = -1;
	resource->owner = -1;
	resource->WaitingListHead = -1;
	resource->waiting_count = 0;

	for (int i = 0; i < MAX_TASK; i++)
	{
		resource->WaitingQueue[i].data = -1;
		resource->WaitingQueue[i].next = -1;
		resource->WaitingQueue[i].prev = -1;
	}
}

bool ReadyResources(int task)
{
	return OSData.Tasks[task].waited_resource == -1;
}

bool ReadyEvents(int task)
{
	bool is_waiting = OSData.Tasks[task].waiting_mask;
	bool is_completed_cond = OSData.Tasks[task].waiting_mask | OSData.Tasks[task].event_mask;
	return !is_waiting || is_completed_cond;
}

int GetMaxPriority(int id)
{
	int max_priority = OSData.Tasks[id].base_priority;
	for (int i = 0; i < MAX_RES; ++i)
	{
		if (OSData.Resources[i].owner == id && OSData.Resources[i].WaitingListHead != -1)
		{
			int head = OSData.Resources[i].WaitingListHead;
			int task = OSData.Resources[i].WaitingQueue[head].data;
			int priority = OSData.Tasks[task].current_priority;
			max_priority = max_priority >= priority ? max_priority : priority;
		}
	}
	return max_priority;
}

void AddTaskToWaitlist(int task_id, int resource_id)
{
	if (OSData.Resources[resource_id].WaitingListHead == -1)
	{
		OSData.Resources[resource_id].WaitingListHead = 0;
		OSData.Resources[resource_id].WaitingQueue[0].data = task_id;
		OSData.Resources[resource_id].WaitingQueue[0].next = -1;
		OSData.Resources[resource_id].WaitingQueue[0].prev = -1;
	}
	else
	{
		ListNode* list_data = OSData.Resources[resource_id].WaitingQueue;
		TTask* list_tasks = OSData.Tasks;

		// найти свободную ячейку
		// идём по списку
		// сравниваем приоритеты
		// нашли - ставим между
		
		int free_slot = -1;

		for (int i = 0; i < MAX_TASK; ++i)
		{
			if (list_data[i].data == -1)
			{
				free_slot = i;
				break;
			}
		}

		int task_priority = OSData.Tasks[task_id].current_priority;
		int next_in_queue = OSData.Resources[resource_id].WaitingListHead;
		int next_in_queue_task = list_data[next_in_queue].data;
		int prev_in_queue = -1;

		while (next_in_queue_task != -1 && OSData.Tasks[next_in_queue_task].current_priority >= task_priority)
		{
			prev_in_queue = next_in_queue;
			next_in_queue = list_data[next_in_queue].next;
			next_in_queue_task = list_data[next_in_queue].data;
		}


		list_data[free_slot].data = task_id;
		list_data[free_slot].next = next_in_queue;
		list_data[free_slot].prev = prev_in_queue;

		if (next_in_queue != -1)
		{
			list_data[next_in_queue].prev = free_slot;
		}
		if (prev_in_queue != -1)
		{
			list_data[prev_in_queue].next = free_slot;
		}
		else
		{
			OSData.Resources[resource_id].WaitingListHead = free_slot;
		}
	}
	return;
}

void RemoveTaskFromQueue(int task_id)
{
	int curr = OSData.TaskListHead;

	while (OSData.TasksQueue[curr].data != task_id)
	{
		curr = OSData.TasksQueue[curr].next;
	}

	int next = OSData.TasksQueue[curr].next;
	int prev = OSData.TasksQueue[curr].prev;

	if (next != -1)
	{
		OSData.TasksQueue[next].prev = prev;
	}
	if (prev != -1)
	{
		OSData.TasksQueue[prev].next = next;
	}

	OSData.TasksQueue[curr].data = -1;
	OSData.TasksQueue[curr].next = -1;
	OSData.TasksQueue[curr].prev = -1;

	return;
}

void log(const char* format, TMessage type, ...) {

	if (type >= OSData.LogLevel)
	{
		printf("--");
		switch (type)
		{
		case message_log:
			if (type >= message_log)
				printf("[LOG] ");
			break;
		case message_info:
			if (type >= message_info)
				printf("[INFO] ");
			break;
		case message_warning:
			if (type >= message_warning)
				printf("--[WARNING] ");
			break;
		case message_error:
			if (type >= message_error)
				printf("--[ERROR] ");
			break;
		}

		va_list args;
		va_start(args, type);
		vprintf(format, args);
		va_end(args);
		printf("\n");
	}
}

int checkTaskId(TTaskId id)
{
	int res = -1;
	for (int i = 0; i < MAX_TASK; ++i)
	{
		if (OSData.Tasks[i].entry == id)
		{
			res = i;
			break;
		}
	}
	return res;
}
