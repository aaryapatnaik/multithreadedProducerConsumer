#include "../include/boundedBuffer.h"

boundedBuffer sharedBuffer;

void initializeBuffer(void)
{
    sharedBuffer.readIndex = 0;
    sharedBuffer.writeIndex = 0;
    sharedBuffer.itemCount = 0;

    pthread_mutex_init(&sharedBuffer.mutex, NULL);
    pthread_cond_init(&sharedBuffer.notFull, NULL);
    pthread_cond_init(&sharedBuffer.notEmpty, NULL);
}

void destroyBuffer(void)
{
    pthread_mutex_destroy(&sharedBuffer.mutex);
    pthread_cond_destroy(&sharedBuffer.notFull);
    pthread_cond_destroy(&sharedBuffer.notEmpty);
}

void *producerThread(void *argument)
{
    int producerId = *(int *)argument;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = producerId * ITEMS_PER_PRODUCER + i;

        pthread_mutex_lock(&sharedBuffer.mutex);

        while (sharedBuffer.itemCount == BUFFER_SIZE) {
            pthread_cond_wait(&sharedBuffer.notFull, &sharedBuffer.mutex);
        }

        sharedBuffer.values[sharedBuffer.writeIndex] = item;
        sharedBuffer.writeIndex = (sharedBuffer.writeIndex + 1) % BUFFER_SIZE;
        sharedBuffer.itemCount++;

        pthread_cond_signal(&sharedBuffer.notEmpty);
        pthread_mutex_unlock(&sharedBuffer.mutex);
    }

    return NULL;
}

void *consumerThread(void *argument)
{
    int consumerId = *(int *)argument;
    int consumedCount = 0;

    (void)consumerId;

    while (consumedCount < ITEMS_PER_PRODUCER) {
        pthread_mutex_lock(&sharedBuffer.mutex);

        while (sharedBuffer.itemCount == 0) {
            pthread_cond_wait(&sharedBuffer.notEmpty, &sharedBuffer.mutex);
        }

        int item = sharedBuffer.values[sharedBuffer.readIndex];
        sharedBuffer.readIndex = (sharedBuffer.readIndex + 1) % BUFFER_SIZE;
        sharedBuffer.itemCount--;

        pthread_cond_signal(&sharedBuffer.notFull);
        pthread_mutex_unlock(&sharedBuffer.mutex);

        consumedCount++;
        (void)item;
    }

    return NULL;
}