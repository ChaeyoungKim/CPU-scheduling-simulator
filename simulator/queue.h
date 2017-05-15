#pragma once
#include "create_process.h"

#define MAX 10 //ready queue 크기. 프로세스 개수가 0-10개이므로. 생성된 프로세스가 ready queue에 모두 들어갈 수 있다고 가정.
typedef Process Element;
typedef int bool;
#define true 1
#define false 0

typedef struct queue {
	int in, out, size, capacity;
	Element* array;
} Queue;

Queue* createQueue(int capacity);
bool isFull(Queue* q);
bool isEmpty(Queue* q);
void enqueue(Queue* q, Element item);
Element dequeue(Queue* q);
int front(Queue* q);
int rear(Queue* q);
void showQueue(Queue* q);
void sort_by_cpuburst(Queue* q);
void sort_by_priority(Queue* q);
void sort_by_ioburst(Queue* q);
//void showioQueue(Queue* q);