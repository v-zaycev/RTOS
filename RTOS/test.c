/*******************************/
/*           test.c            */
/*******************************/

#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include "rtos_api.h"

DeclareResource(Res1, 1);
DeclareResource(Res2, 2);
DeclareResource(Res3, 3);
DeclareResource(Res4, 4);
DeclareResource(Res5, 5);
DeclareResource(Res6, 6);
DeclareResource(Res7, 7);
DeclareResource(Res8, 8);
DeclareResource(Res9, 9);
DeclareResource(Res10, 10);
DeclareResource(Res11, 11);
DeclareResource(Res12, 12);
DeclareResource(Res13, 13);
DeclareResource(Res14, 14);
DeclareResource(Res15, 15);
DeclareResource(Res16, 16);
DeclareResource(Res17, 17);


DeclareTask(Task1_1, 3);
DeclareTask(Task2_1, 2);
DeclareTask(Task3_1, 1);
DeclareTask(Task4_1, 4);
DeclareTask(Task5_1, 5);

DeclareTask(Task1_2,1);
DeclareTask(Task2_2,2);
DeclareTask(Task3_2,3);


DeclareTask(Task1_3, 5);
DeclareTask(Task2_3, 3);
DeclareTask(Task3_3, 10);
DeclareEvent(Event1, 1);

DeclareTask(Task1_4, 5);
DeclareTask(Task2_4, 4);
DeclareTask(Task3_4, 3);
DeclareTask(Task4_4, 2);
DeclareEvent(Event2, 2);
DeclareEvent(Event3, 3);
DeclareEvent(Event4, 4);

DeclareTask(Task1_5, 5);
DeclareTask(Task2_5, 4);
DeclareTask(Task3_5, 3);
DeclareTask(Task4_5, 2);
DeclareTask(Task5_5, 1);

DeclareTask(Task1_6, 1);

DeclareTask(Task1_7, 5);
DeclareTask(Task2_7, 1);
DeclareTask(Task3_7, 1);
DeclareTask(Task4_7, 1);
DeclareTask(Task5_7, 1);
DeclareTask(Task6_7, 1);
DeclareTask(Task7_7, 1);
DeclareTask(Task8_7, 1);
DeclareTask(Task9_7, 1);
DeclareTask(Task10_7, 1);
DeclareTask(Task11_7, 1);
DeclareTask(Task12_7, 1);
DeclareTask(Task13_7, 1);
DeclareTask(Task14_7, 1);
DeclareTask(Task15_7, 1);
DeclareTask(Task16_7, 1);
DeclareTask(Task17_7, 1);
DeclareTask(Task18_7, 1);
DeclareTask(Task19_7, 1);
DeclareTask(Task20_7, 1);
DeclareTask(Task21_7, 1);
DeclareTask(Task22_7, 1);
DeclareTask(Task23_7, 1);
DeclareTask(Task24_7, 1);
DeclareTask(Task25_7, 1);
DeclareTask(Task26_7, 1);
DeclareTask(Task27_7, 1);
DeclareTask(Task28_7, 1);
DeclareTask(Task29_7, 1);
DeclareTask(Task30_7, 1);
DeclareTask(Task31_7, 1);
DeclareTask(Task32_7, 1);
DeclareTask(Task33_7, 1);

void RunTest(const char* test_name, TTaskId start_task, int priority, const char* task_name) {
	printf("========== Running test: %s ==========\n", test_name);
	StartOS(start_task, priority, task_name, message_warning);
	ShutdownOS();
	printf("========== Test %s completed ==========\n\n", test_name);
}

int main(void)
{
	RunTest("Test1", Task1_1, Task1_1_PRIORITY, "Task1");
	RunTest("Test2", Task1_2, Task1_2_PRIORITY, "Task1");
	RunTest("Test3", Task1_3, Task1_3_PRIORITY, "Task1");
	RunTest("Test4", Task1_4, Task1_4_PRIORITY, "Task1");
	RunTest("Test5", Task1_5, Task1_5_PRIORITY, "Task1");
	RunTest("Test6", Task1_6, Task1_6_PRIORITY, "Task1");
	RunTest("Test7", Task1_7, Task1_7_PRIORITY, "Task1");
	return 0;
}


TASK(Task1_1)
{
	printf("Start Task1\n");
	const char* name = "Task2";

	SYSCALL(ActivateTask(Task3_1, Task3_1_PRIORITY, "Task3"));
	SYSCALL(ActivateTask(Task4_1, Task4_1_PRIORITY, "Task4"));
	SYSCALL(ActivateTask(Task5_1, Task5_1_PRIORITY, "Task5"));
	printf("Finish Task1\n");

	TerminateTask();
}

TASK(Task2_1)
{
	printf("Start Task2\n");
	printf("Finish Task2\n");

	TerminateTask();
}

TASK(Task3_1)
{
	printf("Start Task3\n");
	SYSCALL(ActivateTask(Task2_1, Task2_1_PRIORITY, "Task2"));
	printf("Finish Task3\n");

	TerminateTask();
}

TASK(Task4_1)
{
	printf("Start Task4\n");
	printf("Finish Task4\n");

	TerminateTask();
}

TASK(Task5_1)
{
	printf("Start Task5\n");
	printf("Finish Task5\n");

	TerminateTask();
}



TASK(Task1_2)
{
	printf("Start Task1\n");
	SYSCALL(InitResource(Res1));
	SYSCALL(InitResource(Res2));

	SYSCALL(GetResource(Res1));
	SYSCALL(GetResource(Res2));

	SYSCALL(ActivateTask(Task2_2, Task2_2_PRIORITY, "Task2"));
	SYSCALL(ReleaseResource(Res2));
	SYSCALL(ReleaseResource(Res1));
	printf("Finish Task1\n");

	TerminateTask();
}

TASK(Task2_2)
{
	printf("Start Task2\n");

	SYSCALL(ActivateTask(Task3_2, Task3_2_PRIORITY, "Task3"));

	SYSCALL(GetResource(Res1));
	printf("Task2 Working with Res1\n");
	SYSCALL(ReleaseResource(Res1));

	printf("Finish Task2\n");

	TerminateTask();
}

TASK(Task3_2)
{
	printf("Start Task3\n");

	SYSCALL(GetResource(Res2));
	printf("Task3 Working with Res2\n");
	SYSCALL(ReleaseResource(Res2));

	printf("Finish Task3\n");

	TerminateTask();
}



TASK(Task1_3)
{
	printf("Task1 start\n");

	SYSCALL(ActivateTask(Task2_3, Task2_3_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task3_3, Task3_3_PRIORITY, "Task3"));


	printf("Task1 waiting for event\n");
	SYSCALL(WaitEvent(Event1));
	printf("Task1 event received, continuing\n");
	printf("Task1 finish\n");
	TerminateTask();
}

TASK(Task2_3)
{
	printf("Task2 start\n");
	printf("Task2 finish\n");
	TerminateTask();
}

TASK(Task3_3)
{
	printf("Task3 start\n");
	printf("Task3 setting event for Task1\n");
	SYSCALL(SetEvent(Task1_3, Event1));
	printf("Task3 finish\n");
	TerminateTask();
}



TASK(Task1_4)
{
	printf("Task1 start\n");

	SYSCALL(ActivateTask(Task2_4, Task2_4_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task3_4, Task3_4_PRIORITY, "Task3"));
	SYSCALL(ActivateTask(Task4_4, Task3_4_PRIORITY, "Task4"));

	printf("Task1 waiting for Event3 or Event4\n");
	SYSCALL(WaitEvent(Event3 | Event4));
	printf("Task1 event received, continuing\n");
	SYSCALL(GetEvent(Task1_4, &(OSData.stack[1])));
	
	for (OSData.stack[0] = 1; OSData.stack[0] <= MAX_EVENTS; ++OSData.stack[0]) {
		if (OSData.stack[1] & (1 << OSData.stack[0])) {
			printf("Event%d active for Task1\n", OSData.stack[0]);
		}
	}

	SYSCALL(ClearEvent(Event2));

	SYSCALL(GetEvent(Task1_4, &(OSData.stack[1])));

	for (OSData.stack[0] = 1; OSData.stack[0] <= MAX_EVENTS; ++OSData.stack[0]) {
		if (OSData.stack[1] & (1 << OSData.stack[0])) {
			printf("Event%d active for Task1\n", OSData.stack[0]);
		}
	}

	printf("Task1 finish\n");
	TerminateTask();
}

TASK(Task2_4)
{
	printf("Task2 start\n");
	printf("Task3 setting Event1 for Task1\n");
	SYSCALL(SetEvent(Task1_4, Event1));
	printf("Task2 finish\n");
	TerminateTask();
}

TASK(Task3_4)
{
	printf("Task3 start\n");
	printf("Task3 setting Event2 for Task1\n");
	SYSCALL(SetEvent(Task1_4, Event2));
	printf("Task3 finish\n");
	TerminateTask();
}

TASK(Task4_4)
{
	printf("Task4 start\n");
	printf("Task4 setting Event3 for Task1\n");
	SYSCALL(SetEvent(Task1_4, Event3));
	printf("Task4 finish\n");
	TerminateTask();
}



TASK(Task1_5)
{
	printf("Task1 start\n");
	
	SYSCALL(InitResource(Res1));
	SYSCALL(GetResource(Res1));

	SYSCALL(ActivateTask(Task5_5, Task5_5_PRIORITY, "Task5"));
	SYSCALL(ActivateTask(Task3_5, Task3_5_PRIORITY, "Task3"));
	SYSCALL(ActivateTask(Task4_5, Task4_5_PRIORITY, "Task4"));
	SYSCALL(ActivateTask(Task2_5, Task2_5_PRIORITY, "Task2"));

	SYSCALL(WaitEvent(Event1));

	SYSCALL(ReleaseResource(Res1));
	printf("Task1 finish\n");
	TerminateTask();
}

TASK(Task2_5)
{
	printf("Task2 start\n");
	SYSCALL(GetResource(Res1));
	printf("Task2 working with Res1\n");
	SYSCALL(ReleaseResource(Res1));
	printf("Task2 finish\n");

	TerminateTask();
}

TASK(Task3_5)
{
	printf("Task3 start\n");
	SYSCALL(GetResource(Res1));
	printf("Task3 working with Res1\n");
	SYSCALL(ReleaseResource(Res1));
	printf("Task3 finish\n");

	TerminateTask();
}

TASK(Task4_5)
{
	printf("Task4 start\n");
	SYSCALL(GetResource(Res1));
	printf("Task4 working with Res1\n");
	SYSCALL(ReleaseResource(Res1));
	printf("Task4 finish\n");

	TerminateTask();
}

TASK(Task5_5)
{
	printf("Task5 start\n");
	printf("Task5 setting Event1 for Task1\n");
	SYSCALL(SetEvent(Task1_5, Event1));
	printf("Task5 finish\n");
	TerminateTask();
}



TASK(Task1_6)
{
	printf("Task5 start\n");
	SYSCALL(InitResource(Res1));
	SYSCALL(InitResource(Res2));
	SYSCALL(InitResource(Res3));
	SYSCALL(InitResource(Res4));
	SYSCALL(InitResource(Res5));
	SYSCALL(InitResource(Res6));
	SYSCALL(InitResource(Res7));
	SYSCALL(InitResource(Res8));
	SYSCALL(InitResource(Res9));
	SYSCALL(InitResource(Res10));
	SYSCALL(InitResource(Res11));
	SYSCALL(InitResource(Res12));
	SYSCALL(InitResource(Res13));
	SYSCALL(InitResource(Res14));
	SYSCALL(InitResource(Res15));
	SYSCALL(InitResource(Res16));
	SYSCALL(InitResource(Res17));

	SYSCALL(GetResource(Res1));
	SYSCALL(GetResource(Res2));
	SYSCALL(GetResource(Res3));
	SYSCALL(GetResource(Res4));
	SYSCALL(GetResource(Res5));
	SYSCALL(GetResource(Res6));
	SYSCALL(GetResource(Res7));
	SYSCALL(GetResource(Res8));
	SYSCALL(GetResource(Res9));
	SYSCALL(GetResource(Res10));
	SYSCALL(GetResource(Res11));
	SYSCALL(GetResource(Res12));
	SYSCALL(GetResource(Res13));
	SYSCALL(GetResource(Res14));
	SYSCALL(GetResource(Res15));
	SYSCALL(GetResource(Res16));
	SYSCALL(GetResource(Res17));

	SYSCALL(ReleaseResource(Res1));
	SYSCALL(ReleaseResource(Res2));
	SYSCALL(ReleaseResource(Res3));
	SYSCALL(ReleaseResource(Res4));
	SYSCALL(ReleaseResource(Res5));
	SYSCALL(ReleaseResource(Res6));
	SYSCALL(ReleaseResource(Res7));
	SYSCALL(ReleaseResource(Res8));
	SYSCALL(ReleaseResource(Res9));
	SYSCALL(ReleaseResource(Res10));
	SYSCALL(ReleaseResource(Res11));
	SYSCALL(ReleaseResource(Res12));
	SYSCALL(ReleaseResource(Res13));
	SYSCALL(ReleaseResource(Res14));
	SYSCALL(ReleaseResource(Res15));
	SYSCALL(ReleaseResource(Res16));
	SYSCALL(ReleaseResource(Res17));

	printf("Task5 finish\n");
	TerminateTask();
}



TASK(Task1_7)
{
	printf("Task1 start\n");
	SYSCALL(ActivateTask(Task2_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task2_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task3_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task4_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task5_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task6_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task7_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task8_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task9_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task10_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task11_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task12_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task13_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task14_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task15_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task16_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task17_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task18_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task19_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task20_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task21_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task22_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task23_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task24_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task25_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task26_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task27_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task28_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task29_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task30_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task31_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task32_7, Task2_7_PRIORITY, "Task2"));
	SYSCALL(ActivateTask(Task33_7, Task2_7_PRIORITY, "Task2"));
	printf("Task1 finish\n");
	TerminateTask();
}

TASK(Task2_7)
{
	printf("Task2 completed\n");
	TerminateTask();
}

TASK(Task3_7)
{
	printf("Task3 completed\n");
	TerminateTask();
}

TASK(Task4_7)
{
	printf("Task4 completed\n");
	TerminateTask();
}

TASK(Task5_7)
{
	printf("Task5 completed\n");
	TerminateTask();
}

TASK(Task6_7)
{
	printf("Task6 completed\n");
	TerminateTask();
}

TASK(Task7_7)
{
	printf("Task7 completed\n");
	TerminateTask();
}

TASK(Task8_7)
{
	printf("Task8 completed\n");
	TerminateTask();
}

TASK(Task9_7)
{
	printf("Task9 completed\n");
	TerminateTask();
}

TASK(Task10_7)
{
	printf("Task10 completed\n");
	TerminateTask();
}

TASK(Task11_7)
{
	printf("Task11 completed\n");
	TerminateTask();
}

TASK(Task12_7)
{
	printf("Task12 completed\n");
	TerminateTask();
}

TASK(Task13_7)
{
	printf("Task13 completed\n");
	TerminateTask();
}

TASK(Task14_7)
{
	printf("Task14 completed\n");
	TerminateTask();
}

TASK(Task15_7)
{
	printf("Task15 completed\n");
	TerminateTask();
}

TASK(Task16_7)
{
	printf("Task16 completed\n");
	TerminateTask();
}

TASK(Task17_7)
{
	printf("Task17 completed\n");
	TerminateTask();
}

TASK(Task18_7)
{
	printf("Task18 completed\n");
	TerminateTask();
}

TASK(Task19_7)
{
	printf("Task19 completed\n");
	TerminateTask();
}

TASK(Task20_7)
{
	printf("Task20 completed\n");
	TerminateTask();
}

TASK(Task21_7)
{
	printf("Task21 completed\n");
	TerminateTask();
}

TASK(Task22_7)
{
	printf("Task22 completed\n");
	TerminateTask();
}

TASK(Task23_7)
{
	printf("Task23 completed\n");
	TerminateTask();
}

TASK(Task24_7)
{
	printf("Task24 completed\n");
	TerminateTask();
}

TASK(Task25_7)
{
	printf("Task25 completed\n");
	TerminateTask();
}

TASK(Task26_7)
{
	printf("Task26 completed\n");
	TerminateTask();
}

TASK(Task27_7)
{
	printf("Task27 completed\n");
	TerminateTask();
}

TASK(Task28_7)
{
	printf("Task28 completed\n");
	TerminateTask();
}

TASK(Task29_7)
{
	printf("Task29 completed\n");
	TerminateTask();
}

TASK(Task30_7)
{
	printf("Task30 completed\n");
	TerminateTask();
}

TASK(Task31_7)
{
	printf("Task31 completed\n");
	TerminateTask();
}

TASK(Task32_7)
{
	printf("Task32 completed\n");
	TerminateTask();
}

TASK(Task33_7)
{
	printf("Task33 completed\n");
	TerminateTask();
}