/****************************************/
/*               sys.h                   /          
/****************************************/
#ifndef SYS_H
#define SYS_H

#include <setjmp.h>
#include "defs.h"

typedef enum {
  task_running, 
  task_waiting,
  task_ready,
  task_suspended
} TStatus;

typedef enum {
  message_info,
  message_Log,
  message_warning,
  message_error
} TMessage;

typedef void TTaskId(void);

typedef unsigned int TResourceId;

typedef unsigned int TEventMask;

typedef struct {
  int next;
  int data;
  int prev;
} ListNode;

typedef struct {
  void (*entry)(void);
  const char* name;

  int base_priority;
  int current_priority;

  TStatus status;

  TEventMask event_mask;
  TEventMask waiting_mask;
  
  int waited_resource;  

  unsigned int stack[STACK_SIZE];
  jmp_buf buff;
  int jmp_flag;

} TTask;

typedef struct {
  TResourceId id;
  int owner;
  int WaitingListHead;
  ListNode WaitingQueue[MAX_TASK];
} TResource;

typedef struct {
  int RunningTask;
  int TaskListHead;
  TTask Tasks[MAX_TASK];
  ListNode TasksQueue[MAX_TASK];

  int ResourcesCount;
  TResource Resources[MAX_RES];

  unsigned int* stack;

  jmp_buf buff;
  TMessage LogLevel;
} TOS;

extern TOS OSData;

void Schedule(int task);

void Dispatch();

int ReadyResources(int task);

int ReadyEvents(int task);

void EmptyTask(TTask* task);

void EmptyResource(TResource* resource);

int GetMaxPriority(int id);

void AddTaskToWaitlist(int task_id, int resource_id);

void RemoveTaskFromQueue(int task_id);

void Log(const char* format, TMessage type, ...);

int checkTaskId(TTaskId id);


#endif
