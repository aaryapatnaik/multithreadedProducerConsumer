CC = clang
CFLAGS = -Wall -Wextra -std=c11
TARGET = producerConsumer

SRC = src/main.c src/mutexBuffer.c

all:
	$(CC) $(CFLAGS) $(SRC) -lpthread -o $(TARGET)

clean:
	rm -f $(TARGET)