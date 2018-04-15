CXX := g++
CFLAGS = -std=c++11
LDFLAG = -lpthread

OBJECTS_CPP := $(shell ls *.cpp)
TARGET := $(subst .cpp, ,$(OBJECTS_CPP))



all: $(TARGET)

% : %.cpp
	$(CXX) $< $(CFLAGS) $(LDFLAG) -o $@


clean:
	-rm $(TARGET)





