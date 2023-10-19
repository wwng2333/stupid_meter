#include "Queue.h"
#include "math.h"

void enqueue(struct Queue* queue, float item) {
	//SEGGER_RTT_printf(0, "enqueue=%f\n", item);
	if (item > queue->max) queue->max = item;
	if ((queue->rear + 1) % SIZE == queue->front) {
		queue->front = (queue->front + 1) % SIZE;
	}
	queue->arr[queue->rear] = item;
	queue->rear = (queue->rear + 1) % SIZE;
}

int dequeue(struct Queue* queue) {
    if (queue->front == queue->rear) {
        SEGGER_RTT_printf(0, "Queue is empty\n");
        return -1;
    }
    int item = queue->arr[queue->front];
    queue->front = (queue->front + 1) % SIZE;
    return item;
}

void printQueue(struct Queue* queue) {
    int i = queue->front;
		uint8_t x = 0;
		uint8_t y = 0;
    while (i != queue->rear) {
			y = (uint8_t) (80-(queue->arr[i] / queue->max * 66));
			LCD_DrawPoint(x, y, WHITE);
			//SEGGER_RTT_printf(0, "max=%f ", queue->max);
			//SEGGER_RTT_printf(0, "arr[%d]=%f", i, queue->arr[i]);
			//SEGGER_RTT_printf(0, "x=%d, y=%d\r\n", x, y);
			i = (i + 1) % SIZE;
			x++;
    }
    //SEGGER_RTT_printf(0, "\n");
}