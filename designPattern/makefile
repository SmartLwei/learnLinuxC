export CXX := g++
CFLAGS = -std=c++11
LDFLAG = -lpthread -ldl
SUBDIRS = $(shell ls -d */)

clean:
	for a in $(SUBDIRS);\
		do $(MAKE) -C $$a clean; done;
