#include "queue.h"

void QueueInit(Queue_t *Q, uint32_t size, void *Buffer)
{
    Q->Buffer = Buffer;
    Q->head = 0;
    Q->tail = -1;
    Q->size = size;
    Q->count = 0;
}

bool Enqueue(Queue_t *Q, void *item, uint32_t size)
{
    if(QueueFull(Q))
    {
        return false;
    }

    if(Q->tail == Q->size - 1)
    {
        Q->tail = -1;
    }

    Q->tail += size;
    Q->Buffer = (uint8_t*)Q->Buffer + Q->tail;
    memcpy(Q->Buffer, item, size);
    Q->count += size;

    return true;
}

bool Dequeue(Queue_t *Q, void *item, uint32_t size)
{
    if(QueueEmpty(Q))
    {
        return false;
    }

    memcpy(item, Q->Buffer, size);
    Q->head += size;
    Q->Buffer = (uint8_t*)Q->Buffer + Q->head;

    if(Q->head == Q->size)
    {
        Q->head = 0;
    }

    Q->count -= size;

    return true;
}

bool QueueEmpty(Queue_t *Q)
{
    return Q->count == 0 ? true : false;
}

bool QueueFull(Queue_t *Q)
{
    return Q->count == Q->size ? true : false;
}

uint32_t QueueCount(Queue_t *Q)
{
    return Q->head - Q->tail;
}




