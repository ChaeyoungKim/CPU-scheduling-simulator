#pragma once

#include <stdio.h>
#include <malloc.h>
#include "create_process.h"
#include "queue.h"

void FCFS(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double*, double*);
void SJF_NP(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double*, double*);
void SJF_P(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double*, double*);
void Priority_NP(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double*, double*);
void Priority_P(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double*, double*);
void RR(Process* process, Queue* ready_queue, Queue* waiting_queue, int n, double*, double*);
void evaluation(double*, double*, double*, double*, double*, double*, double*, double*, double*, double*, double*, double*);