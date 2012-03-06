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

#definable user
ifeq ($(DISTUSER),)
DISTUSER:=mjs010200
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
	rm -rf $(DISTUSER) $(DISTUSER).zip
	rm -rf pexpect-2.3
	rm -rf .log

dist:
	rm -rf $(DISTUSER) $(DISTUSER).zip
	mkdir -p $(DISTUSER)
	cp *.cpp README Makefile $(DISTUSER)/
	zip $(DISTUSER).zip $(DISTUSER)/*
