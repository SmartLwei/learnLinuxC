CXX := g++
CFLAGS =
LDFLAG = 
SUBDIRS = $(shell ls -d */)

OBJECTS_CPP := $(shell ls *.cpp)
TARGET := $(subst .cpp, ,$(OBJECTS_CPP))



all: $(TARGET)

% : %.cpp
	$(CXX) $< $(LDFLAG) -o $@


clean:
	for a in $(SUBDIRS);\
		do $(MAKE) -C $$a clean; done;
	-rm $(TARGET)





