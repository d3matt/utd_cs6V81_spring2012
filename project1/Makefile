CC=gcc
CXX=g++
LD=$(CXX)
CFLAGS=-Wall -g -fexceptions -O2 -fno-guess-branch-probability
#uncomment to enable extra debug messages
#CFLAGS+=-DPROJ_DEBUG
LDFLAGS=-lpthread

#pretty printing of CXX and LD
ifeq ($(HIDE),)
HIDE:= @
endif

BINLIST=part1 part2 part2a part3

COMMON_OBJECTS = 

default: $(BINLIST)

# default compile rule
.cpp.o:
	@ mkdir -p .depend
	@ echo CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c -o $@ -MMD -MF $(@:%.o=.depend/%.d) $(firstword $^)

#include generated dependency files
-include .depend/*.d

part1: part1.o
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -o $@ $^

part2: part2.o
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -o $@ $^

#part2a uses the gcc built-in __sync_add_and_fetch() instead of a mutex
part2a: part2a.o
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -o $@ $^

part3: part3.o
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.map *.o $(BINLIST) core.* *.exe.stackdump *.pyc

distclean: clean
	rm -rf .depend
	rm -rf mjs010200 mjs010200.zip
	rm -rf pexpect-2.3
	rm -rf .log

dist:
	rm -rf mjs010200 mjs010200.zip
	mkdir -p mjs010200
	cp *.cpp README Makefile mjs010200/
	zip mjs010200.zip mjs010200/*
