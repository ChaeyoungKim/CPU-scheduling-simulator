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
	double wt1 = 9999, tt1 = 9999, wt2 = 9999, tt2 = 9999, wt3 = 9999, tt3 = 9999, wt4 = 9999, tt4 = 9999, wt5 = 9999, tt5 = 9999, wt6 = 9999, tt6 = 9999;
	double *WT1 = &wt1, *TT1 = &tt1, *WT2 = &wt2, *TT2 = &tt2, *WT3 = &wt3, *TT3 = &tt3, *WT4 = &wt4, *TT4 = &tt4, *WT5 = &wt5, *TT5 = &tt5, *WT6 = &wt6, *TT6 = &tt6;
	int n = (rand() % 10) + 1; //the number of processes is 1-10
	Process* process = Create_Process(n);
	
	Queue* ready_queue = createQueue(100);
	Queue* waiting_queue = createQueue(100);

	while (true) {
		printf("\n- - - - - - - - - - - - - - - - - - - - - -\n");
		printf("1. FCFS       2. Non-Preemptive SJF       3. Preemptive SJF\n4. Non-Preemptive Priority       5. Preemptive Priority       6. Round Robin\n7. Evaluation(Only after you did 1~6, you can see the result.)\n8. Create new processes\n\n");
		printf("Enter a number : ");
		scanf("%d", &choice);

		switch (choice)
		{
			case 1:
			{
				FCFS(process, ready_queue, waiting_queue, n, &wt1, &tt1);
				break;
			}
			case 2:
			{
				SJF_NP(process, ready_queue, waiting_queue, n, &wt2, &tt2);
				break;
			}
			case 3:
			{
				SJF_P(process, ready_queue, waiting_queue, n, &wt3, &tt3);
				break;
			}
			case 4:
			{
				Priority_NP(process, ready_queue, waiting_queue, n, &wt4, &tt4);
				break;
			}
			case 5:
			{
				Priority_P(process, ready_queue, waiting_queue, n, &wt5, &tt5);
				break;
			}
			case 6:
			{
				RR(process, ready_queue, waiting_queue, n, &wt6, &tt6);
				break;
			}
			case 7: {
				evaluation(&wt1, &tt1, &wt2, &tt2, &wt3, &tt3, &wt4, &tt4, &wt5, &tt5, &wt6, &tt6);
				break;
			}
			case 8:
			{
				*WT1 = 9999, *TT1 = 9999, *WT2 = 9999, *TT2 = 9999, *WT3 = 9999, *TT3 = 9999, *WT4 = 9999, *TT4 = 9999, *WT5 = 9999, *TT5 = 9999, *WT6 = 9999, *TT6 = 9999;
				n = (rand() % 10) + 1; //the number of processes is 1-10
				process = Create_Process(n);

				ready_queue = createQueue(100);
				waiting_queue = createQueue(100);

				break;
			}
		}
	}
	return 0;
}
