#ifndef BOUNDED_BUFFER_H
#define BOUNDED_BUFFER_H

#include "common.h"

typedef struct {
    int values[BUFFER_SIZE];
    int readIndex;
    int writeIndex;
    int itemCount;
    pthread_mutex_t mutex;
    pthread_cond_t notFull;
    pthread_cond_t notEmpty;
} boundedBuffer;

extern boundedBuffer sharedBuffer;

void initializeBuffer(void);
void destroyBuffer(void);
void *producerThread(void *argument);
void *consumerThread(void *argument);

void initializeSemaphoreBuffer(void);
void destroySemaphoreBuffer(void);
void *semaphoreProducerThread(void *argument);
void *semaphoreConsumerThread(void *argument);

void initializeSpinlockBuffer(void);
void destroySpinlockBuffer(void);
void *spinlockProducerThread(void *argument);
void *spinlockConsumerThread(void *argument);

#endif