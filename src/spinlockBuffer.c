#include "../include/boundedBuffer.h"

static boundedBuffer spinlockBuffer;
static atomic_flag spinlock = ATOMIC_FLAG_INIT;

void lockSpinlock(void)
{
    while (atomic_flag_test_and_set(&spinlock)) {
        // Busy wait until the lock becomes available
    }
}

void unlockSpinlock(void)
{
    atomic_flag_clear(&spinlock);
}

void initializeSpinlockBuffer(void)
{
    spinlockBuffer.readIndex = 0;
    spinlockBuffer.writeIndex = 0;
    spinlockBuffer.itemCount = 0;

    atomic_flag_clear(&spinlock);
}

void destroySpinlockBuffer(void)
{
    // no pthread objects to destroy for this simple spinlock version
}

void *spinlockProducerThread(void *argument)
{
    int producerId = *(int *)argument;

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int item = producerId * ITEMS_PER_PRODUCER + i;

        bool inserted = false;

        while (!inserted) {
            lockSpinlock();

            if (spinlockBuffer.itemCount < BUFFER_SIZE) {
                spinlockBuffer.values[spinlockBuffer.writeIndex] = item;
                spinlockBuffer.writeIndex = (spinlockBuffer.writeIndex + 1) % BUFFER_SIZE;
                spinlockBuffer.itemCount++;

                inserted = true;
            }

            unlockSpinlock();
        }
    }

    return NULL;
}

void *spinlockConsumerThread(void *argument)
{
    int consumerId = *(int *)argument;
    int consumedCount = 0;

    (void)consumerId;

    while (consumedCount < ITEMS_PER_PRODUCER) {
        bool consumed = false;

        while (!consumed) {
            lockSpinlock();

            if (spinlockBuffer.itemCount > 0) {
                int item = spinlockBuffer.values[spinlockBuffer.readIndex];
                spinlockBuffer.readIndex = (spinlockBuffer.readIndex + 1) % BUFFER_SIZE;
                spinlockBuffer.itemCount--;

                consumed = true;
                (void)item;
            }

            unlockSpinlock();
        }

        consumedCount++;
    }

    return NULL;
}