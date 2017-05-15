#include <stdio.h>
#include <malloc.h>
#include "simulator.h"

//I/O에서 갓 돌아온 프로세스를 인지하지 못하는 듯.

void Priority_NP(Process* process, Queue* ready_queue, Queue* waiting_queue, int n) {
	int t = 0, i = 0, s = 0, once = 1;
	double total = 0;
	double avg_waiting_time = 0;
	double avg_turnaround_time = 0;
	int final_termination = 0;
	int m = n; // 마지막으로 종료되는 프로세스 추적
	int flag = 0; //프로세스가 빈 ready queue에 들어오거나 termination이나 I/O가 발생한 후 새로운 프로세스가 유입되면 sort해야 한다. 위의 경우 flag를 1로 바꾼다.
	for (i = 0; i<n; i++)
		process[i].waiting_time = 0; //waiting time 초기화

	printf("\n**************************************\n\tNon-Preemptive Priority scheduling\n**************************************\n\n");
	for (i = 0; i < n; i++) {
		if (process[i].arrival_time == 0) {
			if (process[i].io_burst_time == 0) { //I/O burst time이 0이면 I/O가 발생하지 않는 것으로 간주.
				process[i].io_start_time = -1;
			}
			enqueue(ready_queue, process[i]);
			sort_by_priority(ready_queue);
			printf("\nready queue : "); showQueue(ready_queue);
		}
	}
	for (t = 0; t < 100; t++) { //arrival time 순서대로 ready queue에 process를 집어넣는다.	
		flag = 0;
		if (!isEmpty(ready_queue)) {
			if (ready_queue->array[ready_queue->out].io_start_time == 0) {
				ready_queue->array[ready_queue->out].io_start_time--; //I/O 작업이 시작될 프로세스. I/O 작업은 한 번만 이루어지므로 다음에 위의 if문에 걸리지 않게 음수로.
				enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //I/O 작업을 수행하러 가야 해서 ready queue를 빠져나왔고, I/O가 수행되는 동안 프로세스는 waiting queue에서 기다린다.
				dequeue(ready_queue);
				sort_by_priority(ready_queue);
				printf("\nwaiting queue : "); showQueue(waiting_queue);
				printf("\nready queue : "); showQueue(ready_queue);
				if (!isEmpty(ready_queue)) {
					printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
					ready_queue->array[ready_queue->out].cpu_burst_time--;
					ready_queue->array[ready_queue->out].io_start_time--;
					if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
						process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //프로세스의 종료 시각
						m--;
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++; //CPU 작업한 프로세스 제외한 ready queue에 있던 모든 프로세스의 waiting time을 증가시킴. dequeue하기 전에.
						dequeue(ready_queue);
						flag = 1;
						sort_by_priority(ready_queue);
					}
					else {
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
					}

				}
				else
					printf("Pidle(%d) ", t);
			}
			else { //ready queue의 첫 번째 프로세스가 CPU 작업을 할 차례인 경우 (당장 I/O 하지 않는 경우)
				printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
				ready_queue->array[ready_queue->out].cpu_burst_time--;
				ready_queue->array[ready_queue->out].io_start_time--;
				if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
					process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //프로세스의 종료 시각
					m--;
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
					dequeue(ready_queue);
					flag = 1;
					sort_by_priority(ready_queue);
					printf("\nready queue : "); showQueue(ready_queue);
				}
				else {
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
				}
			}
		} //ready queue에 작업이 남은 경우.
		else { //ready queue에 작업이 없는 경우.
			flag = 1; //ready queue에 작업이 없다가 프로세스가 여러 개 들어오면 정렬을 해줘야 한다.
			if (m != 0) // m은 아직 termination하지 못한 프로세스 개수.
				printf("Pidle(%d) ", t);
		}
		if (!isEmpty(waiting_queue)) {
			sort_by_ioburst(waiting_queue);
			for (i = 0; i<waiting_queue->size; i++)
				waiting_queue->array[(waiting_queue->out + i) % waiting_queue->capacity].io_burst_time--; //waiting queue에 있는 모든 프로세스의 I/O 작업이 실행되었을 것이다.
			while (waiting_queue->array[waiting_queue->out].io_burst_time == 0) {
				enqueue(ready_queue, waiting_queue->array[waiting_queue->out]); //I/O burst time이 0이 되면 프로세스가 waiting queue를 빠져나가 ready queue에 다시 줄을 선다. 여러 개 있을 수 있으니 while
				dequeue(waiting_queue);
			}
			printf("\nupdated waiting queue : "); showQueue(waiting_queue);
			printf("\nupdated ready queue : "); showQueue(ready_queue);
		} //waiting_queue에 작업이 남은 경우.
		for (i = 0; i < n; i++) { //t초에서 도착한 프로세스가 있으면 ready_queue에 넣어준다.
			if (process[i].arrival_time == t + 1) {
				if (process[i].io_burst_time == 0) { //I/O burst time이 0이면 I/O가 발생하지 않는 것으로 간주.
					process[i].io_start_time = -1;
				}
				enqueue(ready_queue, process[i]);
				if (flag == 1)
					sort_by_priority(ready_queue);
				printf("\nready queue : "); showQueue(ready_queue);
			}
		}
	} //for문. time.
	printf("\n\n");

	for (i = 0; i < n; i++) {
		process[i].turnaround_time = process[i].termination_time - process[i].arrival_time;
		printf("P%d termination time : %d\n", i, process[i].termination_time);
		printf("P%d turnaround time : %d\n", i, process[i].turnaround_time);
		printf("P%d waiting time : %d\n\n", i, process[i].waiting_time);
		total += process[i].waiting_time;
		if (final_termination < process[i].termination_time)
			final_termination = process[i].termination_time;
	}
	avg_waiting_time = total / n;
	printf("avg_waiting_time : %f\n", avg_waiting_time);

	total = 0;

	for (i = 0; i < n; i++) {
		total += process[i].turnaround_time;
	}
	avg_turnaround_time = total / n;
	printf("avg_turnaround_time : %f\n", avg_turnaround_time);
	//cpu utilization
}
