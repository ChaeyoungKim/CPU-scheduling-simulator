#pragma once
#include "create_process.h"

#define MAX 10 //ready queue ũ��. ���μ��� ������ 0-10���̹Ƿ�. ������ ���μ����� ready queue�� ��� �� �� �ִٰ� ����.
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