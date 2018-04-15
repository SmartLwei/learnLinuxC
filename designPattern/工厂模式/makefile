CXX := g++
LIBS := -ldl
all: factory1 factory2 factory3

factory1: factory1.cpp factory1.h
	$(CXX) $< $(LIBS) -o $@
	
factory2: factory2.cpp factory2.h
	$(CXX) $< $(LIBS) -o $@

factory3: factory3.cpp factory2.h
	$(CXX) $< $(LIBS) -o $@

clean:
	-rm factory1 factory2 factory3
