export CXX
CFLAGS = -std=c++11
LDFLAG = -lpthread -ldl
SUBDIRS = $(shell ls -d */)

OBJECTS_CPP := $(shell ls *.cpp)
TARGET := $(subst .cpp, ,$(OBJECTS_CPP))



all: $(TARGET)

% : %.cpp
	$(CXX) $< $(LDFLAG) -o $@


clean:
	-rm $(TARGET)





