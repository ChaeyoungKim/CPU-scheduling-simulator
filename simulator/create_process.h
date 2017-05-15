#pragma once

typedef struct process {
	int process_id;
	int arrival_time;
	int cpu_burst_time;
	int io_start_time;
	int io_burst_time;
	int priority;
	int waiting_time;
	int termination_time;
	int turnaround_time;
} Process;

Process* Create_Process(int n);