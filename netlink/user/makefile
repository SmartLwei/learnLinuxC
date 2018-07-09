CXX := gcc
TARGETS := user1 user2 

all: $(TARGETS)
	
user1: user1.c
	$(CXX) $^ -o $@
user2: user2.c
	$(CXX) $^ -o $@


clean:
	rm $(TARGETS)
