#include <stdio.h>
#include <malloc.h>
#include "simulator.h"

void Priority_P(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double* WT5, double* TT5) {
	int t = 0, i = 0, s = 0, once = 1;
	double total = 0;
	double avg_waiting_time = 0;
	*WT5 = avg_waiting_time;
	double avg_turnaround_time = 0;
	*TT5 = avg_turnaround_time;
	int final_termination = 0;
	int m = n; //keep track of the last process that terminates
	for (i = 0; i<n; i++)
		process[i].waiting_time = 0; //waiting time initialization

	printf("\n**************************************\n\tPreemptive Priority scheduling\n**************************************\n\n");

	for (t = 0; t < 100; t++) { //put the processes in the ready queue in the order of arrival time.
		for (i = 0; i < n; i++) { //if there are processes that arrived at t, put them in the ready queue
			if (process[i].arrival_time == t) {
				if (process[i].io_burst_time == 0) { //if I/O burst time is 0, consider it as no I/O burst needed
					process[i].io_start_time = -1;
				}
				enqueue(ready_queue, process[i]);
				//printf("\nready queue : "); showQueue(ready_queue);
			}
		}
		if (!isEmpty(ready_queue)) {
			if (ready_queue->array[ready_queue->out].io_start_time == 0) {
				ready_queue->array[ready_queue->out].io_start_time--; //the process that will start I/O soon. Because I/O happens only once, set it as a negative value
				enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //it gets out of ready queue in order to do I/O and it waits at the waiting queue while I/O is done
				dequeue(ready_queue);
				//printf("\nwaiting queue : "); showQueue(waiting_queue);
				//printf("\nready queue : "); showQueue(ready_queue);
				if (!isEmpty(ready_queue)) {
					sort_by_priority(ready_queue); //if sorted right after processes arrive, it could happen that process is moved back in line and can't go to waiting queue. So sorting needed right before CPU burst.
					printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
					ready_queue->array[ready_queue->out].cpu_burst_time--;
					ready_queue->array[ready_queue->out].io_start_time--;
					if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
						process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //process terminates
						m--;
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++; //increment waiting times of all the processes in ready queue except the process that just did CPU burst. before dequeue.
						dequeue(ready_queue);
					}
					else {
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
					}
				}
				else
					printf("Pidle(%d) ", t);
			}
			else { //the first process of ready queue is about to do CPU (No I/O needed at the moment)
				sort_by_priority(ready_queue); //if sorted in advance, it could be moved back in line and not go to waiting queue, so it's sorted right before CPU burst
				printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
				ready_queue->array[ready_queue->out].cpu_burst_time--;
				ready_queue->array[ready_queue->out].io_start_time--;
				if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
					process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //process terminates
					m--;
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
					dequeue(ready_queue);
					//printf("\nready queue : "); showQueue(ready_queue);
				}
				else {
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
				}
			}
		} //some processes in ready queue
		else { //no processes in ready queue
			if (m != 0) //m is the number of processes that have not terminated yet
				printf("Pidle(%d) ", t);
		}
		if (!isEmpty(waiting_queue)) {
			sort_by_ioburst(waiting_queue);
			for (i = 0; i<waiting_queue->size; i++)
				waiting_queue->array[(waiting_queue->out + i) % waiting_queue->capacity].io_burst_time--; //all the processes in waiting queue did I/O
			while (waiting_queue->array[waiting_queue->out].io_burst_time == 0 && !isEmpty(waiting_queue)) {
				enqueue(ready_queue, waiting_queue->array[waiting_queue->out]); //when I/O burst time becomes 0, those processes get out of the waiting queue and line up in the ready queue. Repeat it in case there are many processes that have finished I/O
				dequeue(waiting_queue);
			}
			//printf("\nupdated waiting queue : "); showQueue(waiting_queue);
			//printf("\nupdated ready queue : "); showQueue(ready_queue);
		} //some processes in waiting_queue
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
	*WT5 = total / n;
	printf("avg_waiting_time : %g\n", *WT5);

	total = 0;

	for (i = 0; i < n; i++) {
		total += process[i].turnaround_time;
	}
	*TT5 = total / n;
	printf("avg_turnaround_time : %g\n", *TT5);
}
