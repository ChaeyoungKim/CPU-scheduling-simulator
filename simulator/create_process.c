#include "create_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

Process* Create_Process(int n) { //create n processes
	int i;
	time_t t;
	srand((unsigned)time(&t));

	Process* process = (Process*)malloc(sizeof(Process) * n);
	printf("\n**************************************\n\tNew processes are created.\n**************************************\n\n");
	printf("%d processes are created.\n\n", n);

	for (i = 0; i < n; i++) { //assign random numbers
		process[i].process_id = i; //integer is assigned in pid in the order of creation
		process[i].arrival_time = rand() % 20;
		process[i].cpu_burst_time = (rand() % 15) + 2; //CPU burst time is 2-16
		process[i].io_start_time = (rand() % (process[i].cpu_burst_time-1)) + 1; //'I/O start time' after cpu burst starts, I/O burst starts. And I/O should start before CPU burst is done. When I/O burst time is 0, this value is meaningless.
		process[i].io_burst_time = (rand() % 5); //I/O burst time is 0-4
		process[i].priority = rand() % 7; //less priority number means higher priority
		process[i].waiting_time = 0;
		process[i].termination_time = 0;
		process[i].turnaround_time = 0;
	}

	printf("[Process ID, Arrival time, CPU Burst time, I/O Start time, I/O Burst time, Priority]\n");
	for (i = 0; i < n; i++) {
		printf("[%d %d %d %d %d %d]\n", process[i].process_id, process[i].arrival_time, process[i].cpu_burst_time, process[i].io_start_time, process[i].io_burst_time, process[i].priority);
	}
	printf("\n");
	return process;
}
