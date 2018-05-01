#include "message.h"

Queue_t Queue;
uint32_t QueueBuffer[QUEUE_BUFFER_SIZE];

void(*Message_CallBack)(uint32_t Msg);

uint32_t GetMessage(void)
{
    uint32_t Msg = IDLE_MESSAGE;

    if(QueueCount(&Queue))
    {
        Dequeue(&Queue, (void*)&Msg, sizeof(Msg));
    }

    return Msg;
}

void RegisterMessageCallback(void(*Message_Callback)(uint32_t Msg))
{
    QueueInit(&Queue, QUEUE_BUFFER_SIZE, QueueBuffer);
    Message_CallBack = Message_Callback;
}

void DispatchMessage(uint32_t Msg)
{
    if(Message_CallBack)
    {
        Message_CallBack(Msg);
    }
}

void Hardware_Callback(uint32_t Event)
{
    Enqueue(&Queue, (void*)&Event, sizeof(Event));
}

