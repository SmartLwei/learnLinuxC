CXX := g++
CFLAGS =
LDFLAG = -lpthread

OBJECTS_CPP := $(shell ls *.cpp)
TARGET := $(subst .cpp, ,$(OBJECTS_CPP))

all: $(TARGET)

% : %.cpp
	$(CXX) $< $(LDFLAG) -o $@

clean:
	-rm $(TARGET)
