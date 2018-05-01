#include "queue.h"

void QueueInit(Queue_t *Q, uint32_t size, void *Buffer)
{
    Q->Buffer = Buffer;
    Q->head = 0;
    Q->tail = 0;
    Q->size = size;
}

bool Enqueue(Queue_t *Q, void *item, uint32_t size)
{
    if(QueueFull(Q))
    {
        return false;
    }

    memcpy((uint8_t*)Q->Buffer + Q->head, item, size);
    Q->head = (Q->head + size) % Q->size;

    return true;
}

bool Dequeue(Queue_t *Q, void *item, uint32_t size)
{
    if(QueueEmpty(Q))
    {
        return false;
    }

    memcpy(item, (uint8_t*)Q->Buffer + Q->tail, size);
    Q->tail = (Q->tail + size) % Q->size;

    return true;
}

bool QueueEmpty(Queue_t *Q)
{
    return Q->head == Q->tail ? true : false;
}

bool QueueFull(Queue_t *Q)
{
    return (Q->head + 1) == Q->tail ? true : false;
}

uint32_t QueueCount(Queue_t *Q)
{
    return Q->head - Q->tail;
}




