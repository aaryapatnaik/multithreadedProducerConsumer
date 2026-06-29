#include "../include/boundedBuffer.h"
#include <semaphore.h>
#include <fcntl.h>

static boundedBuffer semaphoreBuffer;
static sem_t *emptySlots;
static sem_t *filledSlots;
static pthread_mutex_t semaphoreMutex;

void initializeSemaphoreBuffer(void)
{
    semaphoreBuffer.readIndex = 0;
    semaphoreBuffer.writeIndex = 0;
    semaphoreBuffer.itemCount = 0;

    sem_unlink("/emptySlotsSemaphore");
    sem_unlink("/filledSlotsSemaphore");

    emptySlots = sem_open("/emptySlotsSemaphore", O_CREAT, 0644, BUFFER_SIZE);
    filledSlots = sem_open("/filledSlotsSemaphore", O_CREAT, 0644, 0);

    if (emptySlots == SEM_FAILED || filledSlots == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&semaphoreMutex, NULL);
}

void destroySemaphoreBuffer(void)
{
    sem_close(emptySlots);
    sem_close(filledSlots);

    sem_unlink("/emptySlotsSemaphore");
    sem_unlink("/filledSlotsSemaphore");

    pthread_mutex_destroy(&semaphoreMutex);
}

void *semaphoreProducerThread(void *argument)
{
    int producerId = *(int *)argument;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = producerId * ITEMS_PER_PRODUCER + i;

        sem_wait(emptySlots);

        pthread_mutex_lock(&semaphoreMutex);
        semaphoreBuffer.values[semaphoreBuffer.writeIndex] = item;
        semaphoreBuffer.writeIndex = (semaphoreBuffer.writeIndex + 1) % BUFFER_SIZE;
        semaphoreBuffer.itemCount++;
        pthread_mutex_unlock(&semaphoreMutex);

        sem_post(filledSlots);
    }

    return NULL;
}

void *semaphoreConsumerThread(void *argument)
{
    int consumerId = *(int *)argument;
    int consumedCount = 0;

    (void)consumerId;

    while (consumedCount < ITEMS_PER_PRODUCER) {
        sem_wait(filledSlots);

        pthread_mutex_lock(&semaphoreMutex);
        int item = semaphoreBuffer.values[semaphoreBuffer.readIndex];
        semaphoreBuffer.readIndex = (semaphoreBuffer.readIndex + 1) % BUFFER_SIZE;
        semaphoreBuffer.itemCount--;
        pthread_mutex_unlock(&semaphoreMutex);

        sem_post(emptySlots);

        consumedCount++;
        (void)item;
    }

    return NULL;
}