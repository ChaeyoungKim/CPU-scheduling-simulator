#include "create_process.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <time.h>

Process* Create_Process(int n) { //n���� ���μ����� �����ϴ� �Լ�
	int i;
	time_t t;
	srand((unsigned)time(&t));

	Process* process = (Process*)malloc(sizeof(Process) * n);
	printf("\n**************************************\n\tNew processes created.\n**************************************\n\n");
	printf("%d���� ���μ����� �����Ǿ����ϴ�.\n\n", n);

	for (i = 0; i < n; i++) { //�� ���μ����� arrival time, CPU burst time, priority�� ���� ���� �ο�
		process[i].process_id = i; //PID�� ���μ����� �����ϴ� ������� ������ �ο�
		process[i].arrival_time = rand() % 10;
		process[i].cpu_burst_time = (rand() % 5) + 2; //CPU burst time�� 2-6
		process[i].io_start_time = (rand() % (process[i].cpu_burst_time-1)) + 1; //arrival_time + I/O start time�� I/O ������ ���۵Ǵ� �ð�. CPU burst time�� �ʰ��ϱ� ���� I/O�� ���۵Ǿ�� ��. I/O burst�� 0�� ��� �� ���� �ǹ� ����.
		process[i].io_burst_time = (rand() % 4); //I/O burst time�� 0-4
		process[i].priority = rand() % 7; //priority ���ڰ� �������� �켱������ ����.
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