#include "at32f421_wk_config.h"
#include "Queue.h"
#include "math.h"

extern uint8_t SavedPoint[SIZE];

float maxqueue(struct Queue* queue)
{
	float max = 0.0f;
	int i = queue->front;
	while (i != queue->rear) {
		if(queue->arr[i] > max) max = queue->arr[i];
		i = (i + 1) % SIZE;
	}
	return max;
}

void enqueue(struct Queue* queue, float item) {
	//SEGGER_RTT_printf(0, "enqueue=%f\n", item);
	queue->max = maxqueue(queue);
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

void ClearPrint(void)
{
	int i =0;
	for(i=0;i<SIZE;i++)
	{
		LCD_DrawPoint(i, SavedPoint[i], BLACK);
	}
}

void printQueue(struct Queue* queue) {
    int i = queue->front;
		uint8_t x = 0;
		uint8_t y0,y1 = 0;
    while (i != queue->rear) {
			y0 = (uint8_t) (80-(queue->arr[i] / queue->max * 66));
			//y1 = (uint8_t) (80-(queue->arr[i+1] / queue->max * 66));
			SavedPoint[x] = y0;
			//SavedPoint[x+1] = y1;
			LCD_DrawPoint(x, y0, WHITE);
			//LCD_DrawLine(x,y0,x+1,y1, WHITE);
			//SEGGER_RTT_printf(0, "max=%f ", queue->max);
			//SEGGER_RTT_printf(0, "arr[%d]=%f", i, queue->arr[i]);
			//SEGGER_RTT_printf(0, "x=%d, y=%d\r\n", x, y);
			i = (i + 1) % SIZE;
			x++;
    }
    //SEGGER_RTT_printf(0, "\n");
}