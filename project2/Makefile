CC=gcc
CXX=g++
LD=$(CXX)
CFLAGS=-Wall -g -fexceptions -O2
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

BINLIST=simple_test

COMMON_OBJECTS = TASLock.o TTASLock.o BackoffLock.o ALock.o

default: $(BINLIST)

# default compile rule
.cpp.o:
	@ mkdir -p .depend
	@ echo CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c -o $@ -MMD -MF $(@:%.o=.depend/%.d) $(firstword $^)

#include generated dependency files
-include .depend/*.d

simple_test: simple_test.o $(COMMON_OBJECTS)
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -o $@ $^

clean:
	rm -f *.map *.o $(BINLIST) core.* *.exe.stackdump *.pyc

distclean: clean
	rm -rf .depend
	rm -rf $(DISTUSER) $(DISTUSER).zip
	rm -rf .log

dist:
	rm -rf $(DISTUSER) $(DISTUSER).zip
	mkdir -p $(DISTUSER)
	cp *.cpp README Makefile $(DISTUSER)/
	zip $(DISTUSER).zip $(DISTUSER)/*
