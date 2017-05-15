#include "create_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

Process* Create_Process(int n) { //n개의 프로세스를 생성하는 함수
	int i;
	time_t t;
	srand((unsigned)time(&t));

	Process* process = (Process*)malloc(sizeof(Process) * n);
	printf("\n**************************************\n\tNew processes created.\n**************************************\n\n");
	printf("%d개의 프로세스가 생성되었습니다.\n\n", n);

	for (i = 0; i < n; i++) { //각 프로세스의 arrival time, CPU burst time, priority에 랜덤 숫자 부여
		process[i].process_id = i; //PID는 프로세스를 생성하는 순서대로 정수를 부여
		process[i].arrival_time = rand() % 10;
		process[i].cpu_burst_time = (rand() % 5) + 2; //CPU burst time은 2-6
		process[i].io_start_time = (rand() % (process[i].cpu_burst_time-1)) + 1; //arrival_time + I/O start time이 I/O 실제로 시작되는 시각. CPU burst time을 초과하기 전에 I/O가 시작되어야 함. I/O burst가 0인 경우 이 값은 의미 없음.
		process[i].io_burst_time = (rand() % 4); //I/O burst time은 0-4
		process[i].priority = rand() % 7; //priority 숫자가 작을수록 우선순위가 높다.
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