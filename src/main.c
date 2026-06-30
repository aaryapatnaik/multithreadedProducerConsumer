#include "../include/boundedBuffer.h"

void runBenchmark(
    const char *benchmarkName,
    void (*initializeFunction)(void),
    void (*destroyFunction)(void),
    void *(*producerFunction)(void *),
    void *(*consumerFunction)(void *)
)
{
    initializeFunction();

    pthread_t producers[PRODUCER_COUNT];
    pthread_t consumers[CONSUMER_COUNT];

    int producerIds[PRODUCER_COUNT];
    int consumerIds[CONSUMER_COUNT];

    struct timespec startTime;
    struct timespec endTime;

    clock_gettime(CLOCK_MONOTONIC, &startTime);

    for (int i = 0; i < PRODUCER_COUNT; i++) {
        producerIds[i] = i;

        if (pthread_create(&producers[i], NULL, producerFunction, &producerIds[i]) != 0) {
            perror("pthread_create producer");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < CONSUMER_COUNT; i++) {
        consumerIds[i] = i;

        if (pthread_create(&consumers[i], NULL, consumerFunction, &consumerIds[i]) != 0) {
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

    printf("%s\n", benchmarkName);
    printf("Execution time: %.6f seconds\n", elapsedTime);
    printf("Total items processed: %d\n", totalItems);
    printf("Throughput: %.2f items/second\n\n", throughput);

    destroyFunction();
}

int main(void)
{
    printf("=========================================\n");
    printf("Producer Consumer Benchmark\n");
    printf("=========================================\n\n");

    printf("Buffer size: %d\n", BUFFER_SIZE);
    printf("Producer threads: %d\n", PRODUCER_COUNT);
    printf("Consumer threads: %d\n", CONSUMER_COUNT);
    printf("Items per producer: %d\n\n", ITEMS_PER_PRODUCER);

    runBenchmark(
        "Mutex + Condition Variables",
        initializeBuffer,
        destroyBuffer,
        producerThread,
        consumerThread
    );

    runBenchmark(
        "Semaphores",
        initializeSemaphoreBuffer,
        destroySemaphoreBuffer,
        semaphoreProducerThread,
        semaphoreConsumerThread
    );

    runBenchmark(
        "Spinlock",
        initializeSpinlockBuffer,
        destroySpinlockBuffer,
        spinlockProducerThread,
        spinlockConsumerThread
    );

    return 0;
}