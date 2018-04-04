CXX := gcc
TARGETS := net_fac_test net_sec_test 

all: $(TARGETS)
	
net_fac_test: net_fac_test.c
	$(CXX) $^ -o $@
net_sec_test: net_sec_test.c
	$(CXX) $^ -o $@


clean:
	rm $(TARGETS)
