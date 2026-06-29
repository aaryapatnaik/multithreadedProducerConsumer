#include "../include/boundedBuffer.h"

int main(void)
{
    initializeBuffer();

    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_MONOTONIC, &startTime);

    printf("=========================================\n");
    printf("Producer Consumer Benchmark\n");
    printf("=========================================\n\n");

    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Producer threads: %d\n", PRODUCER_COUNT);
    printf("Consumer threads: %d\n", CONSUMER_COUNT);
    printf("Items per producer: %d\n\n", ITEMS_PER_PRODUCER);

    pthread_t producers[PRODUCER_COUNT];
    pthread_t consumers[CONSUMER_COUNT];

    int producerIds[PRODUCER_COUNT];
    int consumerIds[CONSUMER_COUNT];

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        producerIds[i] = i;

        if (pthread_create(&producers[i], NULL, producerThread, &producerIds[i]) != 0) {
            perror("pthread_create producer");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        consumerIds[i] = i;

        if (pthread_create(&consumers[i], NULL, consumerThread, &consumerIds[i]) != 0) {
            perror("pthread_create consumer");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        pthread_join(producers[i], NULL);
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        pthread_join(consumers[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &endTime);

    double elapsedTime =
        (endTime.tv_sec - startTime.tv_sec) +
        (endTime.tv_nsec - startTime.tv_nsec) / 1000000000.0;

    int totalItems = PRODUCER_COUNT * ITEMS_PER_PRODUCER;
    double throughput = totalItems / elapsedTime;

    printf("All producer and consumer threads completed.\n");
    printf("Execution time: %.6f seconds\n", elapsedTime);
    printf("Total items processed: %d\n", totalItems);
    printf("Throughput: %.2f items/second\n", throughput);

    destroyBuffer();

    return 0;
}