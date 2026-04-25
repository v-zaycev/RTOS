/****************************************/
/*           rtos_api.h                 */
/****************************************/
#ifndef RTOS_API_H
#define RTOS_API_H
#include "sys.h"

#define DeclareTask(TaskName, priority)\
	TASK(TaskName);					\
enum { TaskName##_PRIORITY = priority }

#define DeclareResource(ResName,id)\
enum {ResName = id}

#define DeclareEvent(EventName, id)\
enum { EventName = 1 << id }

#define TASK(TaskName) void TaskName(void)

#define SYSCALL(call) \
    do { \
        if (setjmp(OSData.Tasks[OSData.RunningTask].buff) == 0) { \
            OSData.Tasks[OSData.RunningTask].jmp_flag = 1; \
            call; \
        } else { \
        } \
    } while(0)


void ActivateTask(TTaskId entry,int priority, const char* name);
void TerminateTask(void);

int StartOS(TTaskId entry,int priority, const char* name, TMessage logLevel);
void ShutdownOS();

void InitResource(TResourceId id);
void GetResource(TResourceId id);
void ReleaseResource(TResourceId id);

void SetEvent(TTaskId task, TEventMask mask);
void WaitEvent(TEventMask mask);
void GetEvent(TTaskId task, TEventMask* mask);
void ClearEvent(TEventMask mask);

#endif
