#compiler variables
CC = gcc
CFLAGS = -Wall -g
LDFLAGS = -lm

#### Alternate test files locations ####

#fileA = "../torso1/torso1.mtx";
#fileb = "../torso1/b_for_torso1.mtx";
#fileA = "../TSOPF_RS_b678_c2/TSOPF_RS_b678_c2.mtx";
#fileb = "../TSOPF_RS_b678_c2/b_for_TSOPF_RS_b678_c2_b.mtx";
#fileA = "../test_L.mtx";
#fileb = "../test_b.mtx";
########################################

fileA = "../af_0_k101/af_0_k101.mtx"
fileb = "../af_0_k101/b_sparse_af_0_k101.mtx"
filebD = "../af_0_k101/b_dense_af_0_k101.mtx"

objects = mmio.o DAG.o prog.o routines.o

prog: $(objects)
	$(CC) $(LDFLAGS) $(objects) -o $@

test: testSparse testDense

testSparse: prog
	./prog -L $(fileA) -b $(fileb)
	
testDense: prog
	./prog -L $(fileA) -b $(filebD)

.PHONY: clean test
clean:
	$(RM) $(objects) x.mtx prog
