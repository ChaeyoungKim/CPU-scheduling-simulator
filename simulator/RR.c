#include <stdio.h>
#include <malloc.h>
#include "simulator.h"

void RR(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double* WT6, double* TT6) {
	int t = 0, i = 0, s = 0, once = 1, time_quantum = 0, cur_tq = 0;
	Process temp_p;
	temp_p.process_id = 100; //PID�� ���μ����� �����ϴ� ������� ������ �ο�
	temp_p.arrival_time = 0;
	temp_p.cpu_burst_time = 0; //CPU burst time�� 2-6
	temp_p.io_start_time = 0; //arrival_time + I/O start time�� I/O ������ ���۵Ǵ� �ð�. CPU burst time�� �ʰ��ϱ� ���� I/O�� ���۵Ǿ�� ��. I/O burst�� 0�� ��� �� ���� �ǹ� ����.
	temp_p.io_burst_time = 0; //I/O burst time�� 0-4
	temp_p.priority = 0; //priority ���ڰ� �������� �켱������ ����.
	temp_p.waiting_time = 0;
	temp_p.termination_time = 0;
	temp_p.turnaround_time = 0;
	double total = 0;
	double avg_waiting_time = 0;
	*WT6 = avg_waiting_time;
	double avg_turnaround_time = 0;
	*TT6 = avg_turnaround_time;
	int final_termination = 0;
	int m = n; // ���������� ����Ǵ� ���μ��� ����
	int flag = 0; //���μ����� �� ready queue�� �����ų� termination�̳� I/O�� �߻��� �� ���ο� ���μ����� ���ԵǸ� sort�ؾ� �Ѵ�. ���� ��� flag�� 1�� �ٲ۴�.
	for (i = 0; i<n; i++)
		process[i].waiting_time = 0; //waiting time �ʱ�ȭ

	printf("\n**************************************\n\tRR scheduling\n**************************************\n\n");

	printf("Enter time_quantum : ");
	scanf_s("%d", &time_quantum);

	for (i = 0; i < n; i++) {
		if (process[i].arrival_time == 0) {
			if (process[i].io_burst_time == 0) { //I/O burst time�� 0�̸� I/O�� �߻����� �ʴ� ������ ����.
				process[i].io_start_time = -1;
			}
			enqueue(ready_queue, process[i]);
			//printf("\nready queue : "); showQueue(ready_queue);
		}
	}
	cur_tq = time_quantum;
	for (t = 0; t < 100; t++) { //arrival time ������� ready queue�� process�� ����ִ´�.	
		temp_p.process_id = 100; //temp_p ����
		if (!isEmpty(ready_queue)) {
			if (ready_queue->array[ready_queue->out].io_start_time == 0) {
				ready_queue->array[ready_queue->out].io_start_time--; //I/O �۾��� ���۵� ���μ���. I/O �۾��� �� ���� �̷�����Ƿ� ������ ���� if���� �ɸ��� �ʰ� ������.
				enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //I/O �۾��� �����Ϸ� ���� �ؼ� ready queue�� �������԰�, I/O�� ����Ǵ� ���� ���μ����� waiting queue���� ��ٸ���.
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
						process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //���μ����� ���� �ð�
						m--;
						for (i = 1; i<ready_queue->size; i++)
							process[ready_queue->array[(ready_queue->out + i) % ready_queue->capacity].process_id].waiting_time++; //CPU �۾��� ���μ��� ������ ready queue�� �ִ� ��� ���μ����� waiting time�� ������Ŵ. dequeue�ϱ� ����.
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
								ready_queue->array[ready_queue->out].io_burst_time++; //�� time�� �� ���μ����� cpu burst�� I/O �� �� �ϳ��� �����ؾ� �Ǵµ� ���⼭ ������ �� �� �ϰ� �ǹǷ� 1�� ������. (�Ʒ� WQ �ܰ迡�� 1 ����)
								enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //I/O �۾��� �����Ϸ� ���� �ؼ� ready queue�� �������԰�, I/O�� ����Ǵ� ���� ���μ����� waiting queue���� ��ٸ���.
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
			else { //ready queue�� ù ��° ���μ����� CPU �۾��� �� ������ ��� (���� I/O ���� �ʴ� ���)
				printf("P%d(%d) ", ready_queue->array[ready_queue->out].process_id, t);
				ready_queue->array[ready_queue->out].cpu_burst_time--;
				cur_tq--;
				ready_queue->array[ready_queue->out].io_start_time--;
				if (ready_queue->array[ready_queue->out].cpu_burst_time == 0) {
					process[ready_queue->array[(ready_queue->out) % ready_queue->capacity].process_id].termination_time = t + 1; //���μ����� ���� �ð�
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
							ready_queue->array[ready_queue->out].io_burst_time++; //�� time�� �� ���μ����� cpu burst�� I/O �� �� �ϳ��� �����ؾ� �Ǵµ� ���⼭ ������ �� �� �ϰ� �ǹǷ� 1�� ������. (�Ʒ� WQ �ܰ迡�� 1 ����)
							enqueue(waiting_queue, ready_queue->array[ready_queue->out]); //I/O �۾��� �����Ϸ� ���� �ؼ� ready queue�� �������԰�, I/O�� ����Ǵ� ���� ���μ����� waiting queue���� ��ٸ���.
							dequeue(ready_queue);
						}
						else
							temp_p = dequeue(ready_queue);
					}
				}
			}
		} //ready queue�� �۾��� ���� ���.
		else { //ready queue�� �۾��� ���� ���.
			if (m != 0) { // m�� ���� termination���� ���� ���μ��� ����.
				printf("Pidle(%d) ", t);
				//cur_tq = time_quantum;
			}
		}

		if (!isEmpty(waiting_queue)) {
			sort_by_ioburst(waiting_queue);
			for (i = 0; i<waiting_queue->size; i++)
				waiting_queue->array[(waiting_queue->out + i) % waiting_queue->capacity].io_burst_time--; //waiting queue�� �ִ� ��� ���μ����� I/O �۾��� ����Ǿ��� ���̴�.
			while (waiting_queue->array[waiting_queue->out].io_burst_time == 0) {
				enqueue(ready_queue, waiting_queue->array[waiting_queue->out]); //I/O burst time�� 0�� �Ǹ� ���μ����� waiting queue�� �������� ready queue�� �ٽ� ���� ����. ���� �� ���� �� ������ while
				dequeue(waiting_queue);
			}
			//printf("\nupdated waiting queue : "); showQueue(waiting_queue);
			//printf("\nupdated ready queue : "); showQueue(ready_queue);
		} //waiting_queue�� �۾��� ���� ���.

		for (i = 0; i < n; i++) { //t�ʿ��� ������ ���μ����� ������ ready_queue�� �־��ش�.
			if (process[i].arrival_time == t + 1) {
				if (process[i].io_burst_time == 0) { //I/O burst time�� 0�̸� I/O�� �߻����� �ʴ� ������ ����.
					process[i].io_start_time = -1;
				}
				enqueue(ready_queue, process[i]);
				//printf("\n%d�ʿ��� ���μ��� �߰��� �� ready queue : ",t+1); showQueue(ready_queue);
			}
		}
		
		if (m != 0 && temp_p.process_id != 100)
			enqueue(ready_queue, temp_p);
		//printf("\ntime quantum ���� ���μ��� �ٽ� �߰��� �� ready queue : "); showQueue(ready_queue);
		//printf("m : %d\n", m);
	} //for��. time.
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
	//cpu utilization
}
