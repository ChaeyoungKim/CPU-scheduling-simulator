#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "queue.h"

Queue* createQueue(int capacity)
{
	Queue* q = (Queue*)malloc(sizeof(Queue));
	q->capacity = capacity;
	q->in = 0;
	q->out = 0;
	q->size = 0;
	q->array = (Element*)malloc(sizeof(Element) * q->capacity);
	return q;
}

bool isFull(Queue* q)
{
	if (q->size == q->capacity)
		return true;
	else
		return false;
}

bool isEmpty(Queue* q)
{
	if (q->size == 0)
		return true;
	else
		return false;
}

void enqueue(Queue* q, Element item)
{
	if (isFull(q)) {
		printf("The queue is full.\n");
		return;
	}
	q->array[q->in] = item;
	q->in = (q->in + 1) % q->capacity;
	q->size++;
}

Element dequeue(Queue* q)
{
	Element item;
	Process* a = (Process*)malloc(sizeof(Process)); //구조체의 null 만들기
	Process** b = &a; //구조체의 null 만들기
	if (isEmpty(q)) {
		printf("The queue is empty.\n");
		return **b; //구조체의 null을 몰라서 null인 구조체 만듦.
	}
	else {
		item = q->array[q->out];
		q->out = (q->out + 1) % q->capacity;
		q->size--;
		return item;
	}
}

int front(Queue* q)
{
	if (isEmpty(q)) {
		printf("The queue is empty.\n");
		return -1;
	}
	else
		return q->array[q->out].process_id;
}

int rear(Queue* q)
{
	if (isEmpty(q)) {
		printf("The queue is empty.\n");
		return -1;
	}
	else
		return q->array[q->in - 1].process_id;
}

void showQueue(Queue* q)
{
	int i = q->out;
	int size = q->size;
	if (isEmpty(q))
		printf("The queue is empty.\n");
	else {
		while (size) {
			printf("%d ", q->array[i].process_id);
			i = (i + 1) % q->capacity;
			size--;
		}
		printf("\n");
	}
}

void sort_by_cpuburst(Queue* q)
{
	Element* arr;
	arr = (Element*)malloc(sizeof(Element) * q->size);
	int i, j;
	Element temp;
	int size = q->size;

	for (i = 0; i < size; i++) {
		arr[i] = q->array[(q->out + i) % q->capacity];
	}

	for (i = 0; i < size - 1; i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			if (arr[j].cpu_burst_time > arr[j + 1].cpu_burst_time)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;

			}
		}
	}

	for (i = 0; i < size; i++) {
		q->array[(q->out + i) % q->capacity] = arr[i];
	}

	//showQueue(q);
}

void sort_by_priority(Queue* q)
{
	Element* arr;
	arr = (Element*)malloc(sizeof(Element) * q->size);
	int i, j;
	Element temp;
	int size = q->size;

	for (i = 0; i < size; i++) {
		arr[i] = q->array[(q->out + i) % q->capacity];
	}

	for (i = 0; i < size - 1; i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			if (arr[j].priority > arr[j + 1].priority)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;

			}
		}
	}

	for (i = 0; i < size; i++) {
		q->array[(q->out + i) % q->capacity] = arr[i];
	}

	//showQueue(q);
}

void sort_by_ioburst(Queue* q)
{
	Element* arr;
	arr = (Element*)malloc(sizeof(Element) * q->size);
	int i, j;
	Element temp;
	int size = q->size;

	for (i = 0; i < size; i++) {
		arr[i] = q->array[(q->out + i) % q->capacity];
	}

	for (i = 0; i < size - 1; i++)
	{
		for (j = 0; j < size - i - 1; j++)
		{
			if (arr[j].io_burst_time > arr[j + 1].io_burst_time)
			{
				temp = arr[j];
				arr[j] = arr[j + 1];
				arr[j + 1] = temp;

			}
		}
	}

	for (i = 0; i < size; i++) {
		q->array[(q->out + i) % q->capacity] = arr[i];
	}

	//showioQueue(q);
}

/*void showioQueue(Queue* q)
{
	int i = q->out;
	int size = q->size;
	if (isEmpty(q))
		printf("The queue is empty.\n");
	else {
		while (size) {
			printf("%d(%d) ", q->array[i].process_id, q->array[i].io_burst_time);
			i = (i + 1) % q->capacity;
			size--;
		}
		printf("\n");
	}
}*/