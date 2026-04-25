/*********************************/
/*          task.c               */
/*********************************/
#include <stdio.h>
#include <string.h>

#include "sys.h"
#include "rtos_api.h"

//нужно добавить запрет повторной активации задачи
void ActivateTask(TTaskId entry,int priority,const char* name)
{
	log("ActivateTask \"%s\"", message_info, name);

	if (checkTaskId(entry) != -1)
	{
		log("Task \"%s\"s already Activated", message_warning, name);
		return;
	}

	int free_pos = -1;
	for (int i = 0; i < MAX_TASK; ++i)
	{
		if (OSData.Tasks[i].status == task_suspended)
		{
			free_pos = i;
			break;
		}
	}

	if (free_pos == -1)
	{
		log("Max number of tasks already added, skipping action", message_warning);
		return;
	}

	OSData.Tasks[free_pos].base_priority = priority;
	OSData.Tasks[free_pos].current_priority = priority;
	OSData.Tasks[free_pos].entry = entry;
	OSData.Tasks[free_pos].name = name;
	OSData.Tasks[free_pos].status = task_ready;

	Schedule(free_pos);

	// printf("-- End of ActivateTask %s\n",name);

	Dispatch();
}

void TerminateTask(void)
{
	int cur_task = OSData.RunningTask;
	OSData.RunningTask = -1;

	// считаем что в конце занятых ресурсов нет
	//for (int i = 0; i < OSData.Tasks[cur_task].locked_count; ++i)
	//{
	//	
	//}

	log("TerminateTask %s", message_info, OSData.Tasks[cur_task].name);

	EmptyTask(OSData.Tasks + cur_task);

	Dispatch();
}

void Schedule(int task_id)
{
	log("Schedule %s", message_info, OSData.Tasks[task_id].name);

	if (ReadyEvents(task_id) && ReadyResources(task_id))
	{
		int free_slot = -1;

		for (int i = 0; i < MAX_TASK; ++i)
		{
			if (OSData.TasksQueue[i].data == -1)
			{
				free_slot = i;
				break;
			}
		}
		// что если нет пустых слотов
		if (OSData.TaskListHead != -1)
		{
			int task_priority = OSData.Tasks[task_id].current_priority;
			int next_in_queue = OSData.TaskListHead;
			int next_in_queue_task = OSData.TasksQueue[next_in_queue].data;
			int prev_in_queue = -1;

			while (next_in_queue != -1 && OSData.Tasks[next_in_queue_task].current_priority >= task_priority)
			{
				prev_in_queue = next_in_queue;
				next_in_queue = OSData.TasksQueue[next_in_queue].next;
				next_in_queue_task = OSData.TasksQueue[next_in_queue].data;
			}


			OSData.TasksQueue[free_slot].data = task_id;
			OSData.TasksQueue[free_slot].next = next_in_queue;
			OSData.TasksQueue[free_slot].prev = prev_in_queue;
			if (next_in_queue != -1)
			{
				OSData.TasksQueue[next_in_queue].prev = free_slot;
			}
			if (prev_in_queue != -1)
			{
				OSData.TasksQueue[prev_in_queue].next = free_slot;
			}
			else
			{
				OSData.TaskListHead = free_slot;
			}
		}
		else
		{
			OSData.TaskListHead = free_slot;
			OSData.TasksQueue[free_slot].data = task_id;
		}
	}

	// printf("-- End of Schedule %s\n", OSData.Tasks[task_id].name);
}

void Dispatch()
{
	log("Dispatch", message_info);

	if (OSData.TaskListHead != -1)
	{
		if (OSData.RunningTask != -1)
		{
			int current_task = OSData.RunningTask;
			int current_priority = OSData.Tasks[OSData.RunningTask].current_priority;
			int top_priority_task = OSData.TasksQueue[OSData.TaskListHead].data;
			int top_priority = OSData.Tasks[top_priority_task].current_priority;
			if (current_priority < top_priority)
			{
				int next_task = OSData.TasksQueue[OSData.TaskListHead].next;
				OSData.TasksQueue[OSData.TaskListHead].next = -1;
				OSData.TasksQueue[OSData.TaskListHead].data = -1;
				OSData.TasksQueue[OSData.TaskListHead].prev = -1;

				OSData.TaskListHead = next_task;
				OSData.TasksQueue[OSData.TaskListHead].prev = -1;

				OSData.Tasks[current_task].status = task_ready;
				Schedule(current_task);

				OSData.RunningTask = top_priority_task;
				OSData.Tasks[OSData.RunningTask].status = task_running;
				OSData.stack = OSData.Tasks[OSData.RunningTask].stack;
			}
		}
		else
		{
			int top_priority_task = OSData.TasksQueue[OSData.TaskListHead].data;
			int next_task = OSData.TasksQueue[OSData.TaskListHead].next;
			OSData.TasksQueue[OSData.TaskListHead].next = -1;
			OSData.TasksQueue[OSData.TaskListHead].data = -1;
			OSData.TasksQueue[OSData.TaskListHead].prev = -1;

			OSData.TaskListHead = next_task;
			OSData.TasksQueue[OSData.TaskListHead].prev = -1;

			OSData.RunningTask = top_priority_task;
			OSData.Tasks[OSData.RunningTask].status = task_running;
			OSData.stack = OSData.Tasks[OSData.RunningTask].stack;
		}
		log("Running task %s", message_info, OSData.Tasks[OSData.RunningTask].name);
		if (OSData.Tasks[OSData.RunningTask].jmp_flag == -1)
		{
			OSData.Tasks[OSData.RunningTask].jmp_flag = 0;
			OSData.Tasks[OSData.RunningTask].entry();
			return;
		}
		else
		{
			longjmp(OSData.Tasks[OSData.RunningTask].buff, 1);
		}
	}
	if (OSData.RunningTask != -1)
	{
		log("Running task %s", message_info, OSData.Tasks[OSData.RunningTask].name);
	}
	else
	{
		log("No tasks for Running", message_info);
		longjmp(OSData.buff, 1);
	}
}
