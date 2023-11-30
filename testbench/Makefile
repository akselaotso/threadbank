
PROGRAM=as2_mockup
TESTER=as2_testbench
CFLAGS=-O2 -g -Wall -pedantic

all: ${TESTER} ${PROGRAM}

${TESTER}: ${TESTER}.c linebuffer.c

.PHONY: test
test: all
	./${TESTER} ${PROGRAM}

.PHONY: clean
clean:
	rm -rf *.o *~ ${TESTER} ${PROGRAM}
