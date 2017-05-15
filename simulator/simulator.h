#pragma once

#include <stdio.h>
#include <malloc.h>
#include "create_process.h"
#include "queue.h"

void FCFS(Process* process, Queue* ready_queue, Queue* waiting_queue, int n);
void SJF_NP(Process* process, Queue* ready_queue, Queue* waiting_queue, int n);
void SJF_P(Process* process, Queue* ready_queue, Queue* waiting_queue, int n);
void Priority_NP(Process* process, Queue* ready_queue, Queue* waiting_queue, int n);
void Priority_P(Process* process, Queue* ready_queue, Queue* waiting_queue, int n);