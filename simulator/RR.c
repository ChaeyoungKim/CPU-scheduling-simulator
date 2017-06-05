#include <stdio.h>
#include <malloc.h>
#include "simulator.h"

void RR(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double* WT6, double* TT6) {
	int t = 0, i = 0, s = 0, once = 1, time_quantum = 0, cur_tq = 0;
	Process temp_p;
	temp_p.process_id = 100; //PID that distinguishes from real processes
	temp_p.arrival_time = 0;
	temp_p.cpu_burst_time = 0;
	temp_p.io_start_time = 0;
	temp_p.io_burst_time = 0;
	temp_p.priority = 0;
	temp_p.waiting_time = 0;
	temp_p.termination_time = 0;
	temp_p.turnaround_time = 0;

	double total = 0;
	double avg_waiting_time = 0;
	*WT6 = avg_waiting_time;
	double avg_turnaround_time = 0;
	*TT6 = avg_turnaround_time;
	int final_termination = 0;
	int m = n; // keep track of the last process that terminates
	int flag = 0; //sorting needed when processes come into the empty ready queue or new processes come into the ready queue after termination or I/O. In these cases, set flag as 1.
	for (i = 0; i<n; i++)
		process[i].waiting_time = 0; //waiting time initialization­

	printf("\n**************************************\n\tRR scheduling\n**************************************\n\n");

	printf("Enter time quantum : ");
	scanf("%d", &time_quantum);

	for (i = 0; i < n; i++) {
		if (process[i].arrival_time == 0) {
			if (process[i].io_burst_time == 0) { //if I/O burst time is 0, consider it as no I/O needed
				process[i].io_start_time = -1;
			}
			enqueue(ready_queue, process[i]);
			//printf("\nready queue : "); showQueue(ready_queue);
		}
	}
	cur_tq = time_quantum;
	for (t = 0; t < 100; t++) { //put processes in the ready queue in the order of arrival time
		temp_p.process_id = 100; //initialize temp_p
		if (!isEmpty(ready_queue)) {
			if (ready_queue->array[ready_queue->out].io_start_time == 0) {
				ready_queue->array[ready_queue->out].io_start_time--; //the process that will start I/O. Set I/O start time as a negative value because I/O happens only once
				enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //gets out of ready queue to do I/O. While I/O happens, processes wait in the waiting queue.
				dequeue(ready_queue);
				cur_tq = time_quantum;
				//printf("\nwaiting queue : "); showQueue(waiting_queue);
				//printf("\nready queue : "); showQueue(ready_queue);
				if (!isEmpty(ready_queue)) {
					printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
					ready_queue->array[ready_queue->out].cpu_burst_time--;
					cur_tq--;
					ready_queue->array[ready_queue->out].io_start_time--;
					if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
						process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //process terminates
						m--;
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++; //increment waiting times of all the process in the ready queue except the process that just did CPU burst. before dequeue.
						dequeue(ready_queue);
						cur_tq = time_quantum;
					}
					else {
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
						if (cur_tq == 0) {
							cur_tq = time_quantum;
							if (ready_queue->array[ready_queue->out].io_start_time == 0) {
								ready_queue->array[ready_queue->out].io_start_time--;
								ready_queue->array[ready_queue->out].io_burst_time++; //between t and t+1, one process can do either CPU burst or I/O. If it dequeues here, this process will do both. I/O burst times of all the processes in the waiting queue are decremented. So increment I/O burst time here so that even if it's sent to waiting queue now, I/O burst time remains the same.
								enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //gets out of ready queue to go do I/O. Wihle I/O is being done, it waits in the waiting queue
								dequeue(ready_queue);
							}
							else
								temp_p = dequeue(ready_queue);
						}
					}

				}
				else
					printf("Pidle(%d) ", t);
			}
			else { //the first process of the ready queue is about to do CPU burst (No I/O needed at the moment)
				printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
				ready_queue->array[ready_queue->out].cpu_burst_time--;
				cur_tq--;
				ready_queue->array[ready_queue->out].io_start_time--;
				if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
					process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //process terminates
					m--;
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
					dequeue(ready_queue);
					cur_tq = time_quantum;
					//printf("\nready queue : "); showQueue(ready_queue);
				}
				else {
					for (i = 1; i<ready_queue->size; i++)
						process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++;
					if(cur_tq == 0) {
						cur_tq = time_quantum;
						if (ready_queue->array[ready_queue->out].io_start_time == 0) {
							ready_queue->array[ready_queue->out].io_start_time--;
							ready_queue->array[ready_queue->out].io_burst_time++; //between t and t+1, one process can do either CPU burst or I/O. If it dequeues here, this process will do both. I/O burst times of all the processes in the waiting queue are decremented. So increment I/O burst time here so that even if it's sent to waiting queue now, I/O burst time remains the same.
							enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //gets out of ready queue to go do I/O. Wihle I/O is being done, it waits in the waiting queue
							dequeue(ready_queue);
						}
						else
							temp_p = dequeue(ready_queue);
					}
				}
			}
		} //some processes in ready queue
		else { //no processes in ready queue
			if (m != 0) { // m is the number of processes that have not terminated yet
				printf("Pidle(%d) ", t);
			}
		}

		if (!isEmpty(waiting_queue)) {
			sort_by_ioburst(waiting_queue);
			for (i = 0; i<waiting_queue->size; i++)
				waiting_queue->array[(waiting_queue->out + i) % waiting_queue->capacity].io_burst_time--; //all the processes in the waiting queue did I/O
			while (waiting_queue->array[waiting_queue->out].io_burst_time == 0 && !isEmpty(waiting_queue)) {
				enqueue(ready_queue, waiting_queue->array[waiting_queue->out]); //When I/O burst time becomes 0, the process gets out of waiting queue and then go line up in the ready queue. In case there are many processes that just have finished I/O, repeat it.
				dequeue(waiting_queue);
			}
			//printf("\nupdated waiting queue : "); showQueue(waiting_queue);
			//printf("\nupdated ready queue : "); showQueue(ready_queue);
		} //there are processes in the waiting_queue

		for (i = 0; i < n; i++) { //if any processes arrived at t, put them in the ready queue
			if (process[i].arrival_time == t + 1) {
				if (process[i].io_burst_time == 0) { //if I/O burst time is 0, consider it as I/O not needed.
					process[i].io_start_time = -1;
				}
				enqueue(ready_queue, process[i]);
			}
		}
		
		if (m != 0 && temp_p.process_id != 100)
			enqueue(ready_queue, temp_p);
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
	*WT6 = total / n;
	printf("avg_waiting_time : %g\n", *WT6);

	total = 0;

	for (i = 0; i < n; i++) {
		total += process[i].turnaround_time;
	}
	*TT6 = total / n;
	printf("avg_turnaround_time : %g\n", *TT6);
}
