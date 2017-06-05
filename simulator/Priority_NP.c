#include <stdio.h>
#include <malloc.h>
#include "simulator.h"

void Priority_NP(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double* WT4, double* TT4) {
	int t = 0, i = 0, s = 0, once = 1;
	double total = 0;
	double avg_waiting_time = 0;
	*WT4 = avg_waiting_time;
	double avg_turnaround_time = 0;
	*TT4 = avg_turnaround_time;
	int final_termination = 0;
	int m = n; //keep track of the last process that terminates
	int flag = 0; //sort needed only when processes come into an empty ready queue or a new process come into the ready queue after a process terminates or I/O happens. In these cases, set flag as 1.
	for (i = 0; i<n; i++)
		process[i].waiting_time = 0; //initialize waiting time­

	printf("\n**************************************\n\tNon-Preemptive Priority scheduling\n**************************************\n\n");
	for (i = 0; i < n; i++) {
		if (process[i].arrival_time == 0) {
			if (process[i].io_burst_time == 0) { //if I/O burst time is 0, consider it as if no I/O burst happens.
				process[i].io_start_time = -1;
			}
			enqueue(ready_queue, process[i]);
			sort_by_priority(ready_queue);
			//printf("\nready queue : "); showQueue(ready_queue);
		}
	}
	for (t = 0; t < 500; t++) { //put processes in the ready queue in the order of arrival time
		flag = 0;
		if (!isEmpty(ready_queue)) {
			if (ready_queue->array[ready_queue->out].io_start_time == 0) {
				ready_queue->array[ready_queue->out].io_start_time--; //process that will do I/O soon. I/O burst happens only once, so set it as a negative value so that it skips 'if' next time.
				enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //it gets out of ready queue to do I/O and while I/O happens, the process waits in the waiting queue.
				dequeue(ready_queue);
				sort_by_priority(ready_queue);
				//printf("\nwaiting queue : "); showQueue(waiting_queue);
				//printf("\nready queue : "); showQueue(ready_queue);
				if (!isEmpty(ready_queue)) {
					printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
					ready_queue->array[ready_queue->out].cpu_burst_time--;
					ready_queue->array[ready_queue->out].io_start_time--;
					if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
						process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //process terminates
						m--;
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++; //increment waiting times of all the processes in ready queue except the process that just did CPU. Before dequeue.
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
			else { //the first process of ready queue is about to do CPU burst. (No I/O burst needed at the moment
				printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
				ready_queue->array[ready_queue->out].cpu_burst_time--;
				ready_queue->array[ready_queue->out].io_start_time--;
				if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
					process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //process terminates
					m--;
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
					dequeue(ready_queue);
					flag = 1;
					sort_by_priority(ready_queue);
					//printf("\nready queue : "); showQueue(ready_queue);
				}
				else {
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
				}
			}
		} //one or more processes in ready queue
		else { //no processes in ready queue
			if (m != 0) { // m is the number of processes that did not terminate yet
				printf("Pidle(%d) ", t);
				flag = 1; //if some processes come into the ready queue after a while it was empty, they need sorting.
			}
		}

		if (!isEmpty(waiting_queue)) {
			sort_by_ioburst(waiting_queue);
			for (i = 0; i<waiting_queue->size; i++)
				waiting_queue->array[(waiting_queue->out + i) % waiting_queue->capacity].io_burst_time--; //I/O of processes in waiting queue is in progress
			while (waiting_queue->array[waiting_queue->out].io_burst_time == 0 && !isEmpty(waiting_queue)) {
				enqueue(ready_queue, waiting_queue->array[waiting_queue->out]); //when I/O burst time becomes 0, processes goes out of waiting queue and line up in ready queue. In case there are many processes that have finished I/O, repeat it.
				dequeue(waiting_queue);
			}
			if (flag == 1)
				sort_by_priority(ready_queue);
			//printf("\nupdated waiting queue : "); showQueue(waiting_queue);
			//printf("\nupdated ready queue : "); showQueue(ready_queue);
		} //one or more processes in waiting queue
		for (i = 0; i < n; i++) { //if there are processes that arrived at t, put them in ready queue
			if (process[i].arrival_time == t + 1) {
				if (process[i].io_burst_time == 0) { //if I/O burst time is 0, consider it as if I/O is not needed
					process[i].io_start_time = -1;
				}
				enqueue(ready_queue, process[i]);
				if (flag == 1)
					sort_by_priority(ready_queue);
				//printf("\nready queue : "); showQueue(ready_queue);
			}
		}
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
	*WT4 = total / n;
	printf("avg_waiting_time : %g\n", *WT4);

	total = 0;

	for (i = 0; i < n; i++) {
		total += process[i].turnaround_time;
	}
	*TT4 = total / n;
	printf("avg_turnaround_time : %g\n", *TT4);
}
