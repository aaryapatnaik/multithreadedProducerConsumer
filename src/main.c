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