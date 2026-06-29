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

#endif