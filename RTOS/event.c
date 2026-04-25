#include "sys.h"
#include "defs.h"
#include "rtos_api.h"
#include <stdio.h> 

void SetEvent(TTaskId task_id, TEventMask mask)
{
  int id = checkTaskId(task_id);

  Log("SetEvent for task \"%s\"", message_info, OSData.Tasks[id].name);

  OSData.Tasks[id].event_mask |= mask;
  int wait_finished = OSData.Tasks[id].event_mask & OSData.Tasks[id].waiting_mask;
  if (wait_finished && OSData.Tasks[id].status != task_ready)
  {
    OSData.Tasks[id].event_mask &= ~OSData.Tasks[id].waiting_mask;
    OSData.Tasks[id].waiting_mask = 0;
    OSData.Tasks[id].status = task_ready;

    Log("Task \"%s\" Event Happened", message_info, OSData.Tasks[id].name);

    Schedule(id);
    Dispatch();
  }
}

void WaitEvent(TEventMask mask)
{
  int task_id = OSData.RunningTask;
  OSData.Tasks[task_id].waiting_mask = mask;

  Log("Task \"%s\" Waiting Event", message_info, OSData.Tasks[task_id].name);

  if (!(OSData.Tasks[task_id].event_mask & OSData.Tasks[task_id].waiting_mask))
  {
    OSData.Tasks[task_id].status = task_waiting;
    OSData.RunningTask = -1;

    Dispatch();
  }
  else
  {
    Log("Task \"%s\" Event alredy happened", message_info, OSData.Tasks[task_id].name);
    OSData.Tasks[task_id].event_mask &= ~OSData.Tasks[task_id].waiting_mask;
  }
}

void GetEvent(TTaskId task_id, TEventMask* mask)
{
  int id = checkTaskId(task_id);
  Log("GetEvent for task \"%s\"", message_info, OSData.Tasks[id].name);
  
  *mask = OSData.Tasks[id].event_mask;
}

void ClearEvent(TEventMask mask)
{
  Log("ClearEvent for task \"%s\"", message_info, OSData.Tasks[OSData.RunningTask].name);

  OSData.Tasks[OSData.RunningTask].event_mask &= ~mask;
}
