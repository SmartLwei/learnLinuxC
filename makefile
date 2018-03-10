CC := g++
CFLAGS = 
TARGET = pthread_test socket_client_test socket_server_test

all: $(TARGET)

pthread_test: pthread_test.cpp
	$(CC) $< -lpthread -o $@
socket_client_test: socket_client_test.cpp
	$(CC) $< -o $@
socket_server_test: socket_server_test.cpp
	$(CC) $< -o $@
clean:
	-rm $(TARGET)
