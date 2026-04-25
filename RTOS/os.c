/******************************/
/*          os.c              */
/******************************/

#include <stdio.h>
#include "sys.h"
#include "rtos_api.h"

void EmptyTask(TTask* task);
void EmptyResource(TResource* resource);

int StartOS(TTaskId entry,int priority,const char* name, TMessage LogLevel)
{
	OSData.RunningTask = -1;
	OSData.ResourcesCount = 0;
	OSData.TaskListHead = -1;
	OSData.stack = NULL;
	OSData.LogLevel = LogLevel;

	Log("StartOS!", message_info);

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
	Log("ShutdownOS!", message_info);
}
