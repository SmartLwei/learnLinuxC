CC := g++
LIB := -lpthread
all:
	$(CC) main.cpp $(LIB) -o test.out
clean:
	-rm test.out
