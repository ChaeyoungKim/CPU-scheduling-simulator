#include <stdio.h>
#include <malloc.h>
#include "simulator.h"

void FCFS(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double* WT1, double* TT1) {
	int t = 0, i = 0, s = 0;
	double total = 0;
	double avg_waiting_time = 0;
	*WT1 = avg_waiting_time;
	double avg_turnaround_time = 0;
	*TT1 = avg_turnaround_time;
	int final_termination = 0;
	int m = n; // keep track of the last process that terminates
	for(i=0;i<n;i++)
		process[i].waiting_time = 0; //initialize waiting times of all processes?

	printf("\n**************************************\n\tFCFS scheduling\n**************************************\n\n");

	for (t = 0; t < 500; t++) { //put processes in ready queue by the order of arrival time
		//printf("\ntime : %d\n", t);
		for (i = 0; i < n; i++) { //if there is any process that arrived at t, put it in ready queue
			if (process[i].arrival_time == t) {
				if (process[i].io_burst_time == 0) { //if I/O burst time is 0, it is considered as no I/O burst needed
					process[i].io_start_time = -1;
				}
				enqueue(ready_queue, process[i]);
				//printf("\nready queue : "); showQueue(ready_queue);
			}
		}
		if (!isEmpty(ready_queue)) {
			if (ready_queue->array[ready_queue->out].io_start_time == 0) {
				ready_queue->array[ready_queue->out].io_start_time--; //the process that will start I/O burst soon. Because I/O burst happens only once, I/O start time is set as a negative value.
				enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //It went out of ready queue to go do I/O and while I/O happens I/O waits at the waiting queue
				dequeue(ready_queue);
				//printf("\nwaiting queue : "); showQueue(waiting_queue);
				//printf("\nready queue : "); showQueue(ready_queue);
				if (!isEmpty(ready_queue)) {
					printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
					ready_queue->array[ready_queue->out].cpu_burst_time--;
					ready_queue->array[ready_queue->out].io_start_time--;
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++; //increment waiting time of all the processes that were in ready queue except the process that just did CPU burst.
					if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
						process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //process terminates
						m--;
						dequeue(ready_queue);
					}
				}
				else
					printf("Pidle(%d) ", t);
			}
			else { //when the first process of ready queue is about to do CPU burst. (No I/O needed at the moment)
				printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
				ready_queue->array[ready_queue->out].cpu_burst_time--;
				ready_queue->array[ready_queue->out].io_start_time--;
				for (i = 1; i<ready_queue->size; i++)
					process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
				if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
					process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //process terminates
					m--;
					dequeue(ready_queue);
					//printf("\nready queue : "); showQueue(ready_queue);
				}
			}
		} //There are more than one process in ready queue
		else { //no processes in ready queue
			if(m!=0)
				printf("Pidle(%d) ", t);
		}
		if (!isEmpty(waiting_queue)) {
			sort_by_ioburst(waiting_queue);
			for (i = 0; i<waiting_queue->size; i++)
				waiting_queue->array[(waiting_queue->out + i) % waiting_queue->capacity].io_burst_time--;
			while (waiting_queue->array[waiting_queue->out].io_burst_time == 0 && !isEmpty(waiting_queue)) {
				enqueue(ready_queue, waiting_queue->array[waiting_queue->out]); //when I/O burst time becomes 0, those processes go out of waiting queue and get in line of ready queue.
				dequeue(waiting_queue);
			}
			//printf("\nupdated waiting queue : "); showQueue(waiting_queue);
			//printf("\nupdated ready queue : "); showQueue(ready_queue);
		} //one of more process in waiting_queue
	} //for loop. time.
	printf("\n\n");

	for (i = 0; i < n; i++) {
		process[i].turnaround_time = process[i].termination_time - process[i].arrival_time;
		//printf("P%d termination time : %d\n", i, process[i].termination_time);
		//printf("P%d turnaround time : %d\n", i, process[i].turnaround_time);
		//printf("P%d waiting time : %d\n\n", i, process[i].waiting_time);
		total += process[i].waiting_time;
		if (final_termination < process[i].termination_time)
			final_termination = process[i].termination_time;
	}
	*WT1 = total / n;
	printf("avg_waiting_time : %g\n", *WT1);
	total = 0;

	for (i = 0; i < n; i++) {
		total += process[i].turnaround_time;
	}
	*TT1 = total / n;
	printf("avg_turnaround_time : %g\n", *TT1);
}
