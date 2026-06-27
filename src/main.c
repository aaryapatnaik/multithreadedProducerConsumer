#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define BUFFER_SIZE 8
#define PRODUCER_COUNT 4
#define CONSUMER_COUNT 4
#define ITEMS_PER_PRODUCER 100000

typedef struct {
    int values[BUFFER_SIZE];
    int readIndex;
    int writeIndex;
    int itemCount;
    pthread_mutex_t mutex;
    pthread_cond_t notFull;
    pthread_cond_t notEmpty;
} boundedBuffer;

static boundedBuffer sharedBuffer;

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

int main(void)
{
    initializeBuffer();
    printf("=========================================\n");
    printf("Producer Consumer Benchmark\n");
    printf("=========================================\n\n");

    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Producer threads: %d\n", PRODUCER_COUNT);
    printf("Consumer threads: %d\n", CONSUMER_COUNT);
    printf("Items per producer: %d\n\n", ITEMS_PER_PRODUCER);

    destroyBuffer();

    return 0;
}