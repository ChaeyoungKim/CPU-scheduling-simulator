#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "create_process.h"
#include "queue.h"
#include "simulator.h"

int main(void) {
	time_t t;
	srand((unsigned)time(&t));
	int choice = 0;

	int n = (rand() % 2) + 3; //프로세스 개수는 1-10개
	Process* process = Create_Process(n); //show_queue가 process 어떻게 보여줄지
	
	Queue* ready_queue = createQueue(100);
	Queue* waiting_queue = createQueue(100);

	while (true) {
		printf("\n- - - - - - - - - - - - - - - - - - - - - -\n");
		printf("1. FCFS\n2. Non-Preemptive SJF\n3. Preemptive SJF\n4. Non-Preemptive Priority\n5. Preemptive Priority\n6. Round Robin\n7. Create new processes\n\n");
		printf("Enter a number : ");
		scanf_s("%d", &choice);

		switch (choice)
		{
			case 1:
			{
				FCFS(process, ready_queue, waiting_queue, n);
				break;
			}
			case 2:
			{
				SJF_NP(process, ready_queue, waiting_queue, n);
				break;
			}
			case 3:
			{
				SJF_P(process, ready_queue, waiting_queue, n);
				break;
			}
			case 4:
			{
				Priority_NP(process, ready_queue, waiting_queue, n);
				break;
			}
			case 5:
			{
				Priority_P(process, ready_queue, waiting_queue, n);
				break;
			}
			case 6:
			{
				break;
			}
			case 7:
			{
				n = (rand() % 2) + 3; //프로세스 개수는 1-10개
				process = Create_Process(n); //show_queue가 process 어떻게 보여줄지

				ready_queue = createQueue(100);
				waiting_queue = createQueue(100);

				break;
			}
		}
	}
	return 0;
}