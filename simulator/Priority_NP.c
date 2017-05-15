#include <stdio.h>
#include <malloc.h>
#include "simulator.h"

//I/O���� �� ���ƿ� ���μ����� �������� ���ϴ� ��.

void Priority_NP(Process* process, Queue* ready_queue, Queue* waiting_queue, int n) {
	int t = 0, i = 0, s = 0, once = 1;
	double total = 0;
	double avg_waiting_time = 0;
	double avg_turnaround_time = 0;
	int final_termination = 0;
	int m = n; // ���������� ����Ǵ� ���μ��� ����
	int flag = 0; //���μ����� �� ready queue�� �����ų� termination�̳� I/O�� �߻��� �� ���ο� ���μ����� ���ԵǸ� sort�ؾ� �Ѵ�. ���� ��� flag�� 1�� �ٲ۴�.
	for (i = 0; i<n; i++)
		process[i].waiting_time = 0; //waiting time �ʱ�ȭ

	printf("\n**************************************\n\tNon-Preemptive Priority scheduling\n**************************************\n\n");
	for (i = 0; i < n; i++) {
		if (process[i].arrival_time == 0) {
			if (process[i].io_burst_time == 0) { //I/O burst time�� 0�̸� I/O�� �߻����� �ʴ� ������ ����.
				process[i].io_start_time = -1;
			}
			enqueue(ready_queue, process[i]);
			sort_by_priority(ready_queue);
			printf("\nready queue : "); showQueue(ready_queue);
		}
	}
	for (t = 0; t < 100; t++) { //arrival time ������� ready queue�� process�� ����ִ´�.	
		flag = 0;
		if (!isEmpty(ready_queue)) {
			if (ready_queue->array[ready_queue->out].io_start_time == 0) {
				ready_queue->array[ready_queue->out].io_start_time--; //I/O �۾��� ���۵� ���μ���. I/O �۾��� �� ���� �̷�����Ƿ� ������ ���� if���� �ɸ��� �ʰ� ������.
				enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //I/O �۾��� �����Ϸ� ���� �ؼ� ready queue�� �������԰�, I/O�� ����Ǵ� ���� ���μ����� waiting queue���� ��ٸ���.
				dequeue(ready_queue);
				sort_by_priority(ready_queue);
				printf("\nwaiting queue : "); showQueue(waiting_queue);
				printf("\nready queue : "); showQueue(ready_queue);
				if (!isEmpty(ready_queue)) {
					printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
					ready_queue->array[ready_queue->out].cpu_burst_time--;
					ready_queue->array[ready_queue->out].io_start_time--;
					if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
						process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //���μ����� ���� �ð�
						m--;
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++; //CPU �۾��� ���μ��� ������ ready queue�� �ִ� ��� ���μ����� waiting time�� ������Ŵ. dequeue�ϱ� ����.
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
			else { //ready queue�� ù ��° ���μ����� CPU �۾��� �� ������ ��� (���� I/O ���� �ʴ� ���)
				printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
				ready_queue->array[ready_queue->out].cpu_burst_time--;
				ready_queue->array[ready_queue->out].io_start_time--;
				if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
					process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //���μ����� ���� �ð�
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
		} //ready queue�� �۾��� ���� ���.
		else { //ready queue�� �۾��� ���� ���.
			flag = 1; //ready queue�� �۾��� ���ٰ� ���μ����� ���� �� ������ ������ ����� �Ѵ�.
			if (m != 0) // m�� ���� termination���� ���� ���μ��� ����.
				printf("Pidle(%d) ", t);
		}
		if (!isEmpty(waiting_queue)) {
			sort_by_ioburst(waiting_queue);
			for (i = 0; i<waiting_queue->size; i++)
				waiting_queue->array[(waiting_queue->out + i) % waiting_queue->capacity].io_burst_time--; //waiting queue�� �ִ� ��� ���μ����� I/O �۾��� ����Ǿ��� ���̴�.
			while (waiting_queue->array[waiting_queue->out].io_burst_time == 0) {
				enqueue(ready_queue, waiting_queue->array[waiting_queue->out]); //I/O burst time�� 0�� �Ǹ� ���μ����� waiting queue�� �������� ready queue�� �ٽ� ���� ����. ���� �� ���� �� ������ while
				dequeue(waiting_queue);
			}
			printf("\nupdated waiting queue : "); showQueue(waiting_queue);
			printf("\nupdated ready queue : "); showQueue(ready_queue);
		} //waiting_queue�� �۾��� ���� ���.
		for (i = 0; i < n; i++) { //t�ʿ��� ������ ���μ����� ������ ready_queue�� �־��ش�.
			if (process[i].arrival_time == t + 1) {
				if (process[i].io_burst_time == 0) { //I/O burst time�� 0�̸� I/O�� �߻����� �ʴ� ������ ����.
					process[i].io_start_time = -1;
				}
				enqueue(ready_queue, process[i]);
				if (flag == 1)
					sort_by_priority(ready_queue);
				printf("\nready queue : "); showQueue(ready_queue);
			}
		}
	} //for��. time.
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
