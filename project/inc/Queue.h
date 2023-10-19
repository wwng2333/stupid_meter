#ifndef __Queue_H
#define __Queue_H

#include "at32f421_wk_config.h"

#define SIZE 130

struct Queue {
	float arr[SIZE];
	int front, rear;
	float max;
};

void enqueue(struct Queue* queue, float item);
int dequeue(struct Queue* queue);
void printQueue(struct Queue* queue);

#endif