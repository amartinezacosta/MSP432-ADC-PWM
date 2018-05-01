#ifndef STRUCTURES_MESSAGE_H_
#define STRUCTURES_MESSAGE_H_

#include "queue.h"

#define QUEUE_BUFFER_SIZE   512

#define IDLE_MESSAGE        120

uint32_t GetMessage(void);
void RegisterMessageCallback(void(*Message_Callback)(uint32_t Msg));
void DispatchMessage(uint32_t Msg);
void Hardware_Callback(uint32_t Event);

#endif /* STRUCTURES_MESSAGE_H_ */
