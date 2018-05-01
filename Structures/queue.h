#ifndef STRUCTURES_QUEUE_H_
#define STRUCTURES_QUEUE_H_

#include <stdbool.h>
#include <stdint.h>

typedef struct
{
    void *Buffer;
    int32_t head;
    int32_t tail;
    uint32_t size;
    uint32_t count;
}Queue_t;

void QueueInit(Queue_t *Q, uint32_t size, void *Buffer);
bool Enqueue(Queue_t *Q, void *item, uint32_t size);
bool Dequeue(Queue_t *Q, void *item, uint32_t size);
bool QueueEmpty(Queue_t *Q);
bool QueueFull(Queue_t *Q);
uint32_t QueueCount(Queue_t *Q);

#endif /* STRUCTURES_QUEUE_H_ */
