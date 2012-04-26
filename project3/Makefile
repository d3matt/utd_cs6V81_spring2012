CC=gcc
CXX=g++
LD=$(CXX)
CFLAGS=-Wall -g -fexceptions -O2
#uncomment to enable extra debug messages
#CFLAGS+=-DPROJ_DEBUG
LDFLAGS=-lpthread -lrt

#pretty printing of CXX and LD
ifeq ($(HIDE),)
HIDE:= @
endif

#definable user
ifeq ($(DISTUSER),)
DISTUSER:=mjs010200
endif

PYTHON=$(shell ./findpython.sh)

BINLIST=testone

COMMON_OBJECTS=LockStack.o LockFreeStack.o

default: $(BINLIST)

# default compile rule
.cpp.o:
	@ mkdir -p .depend
	@ echo CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c -o $@ -MMD -MF $(@:%.o=.depend/%.d) $(firstword $^)

%.o.prof: %.cpp
	@ mkdir -p .depend
	@ echo CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c pg -o $@ -MMD -MF $(@:%.o.prof=.depend/$.d) $(firstword $^)

#include generated dependency files
-include .depend/*.d

testone: testone.o $(COMMON_OBJECTS)
	@ echo LD $@
	$(HIDE) $(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^

tests test: $(BINLIST)
	$(PYTHON) ./test.py

.PHONY: tests test

pdf:
	@echo PDF $(DISTUSER)
	$(HIDE) $(MAKE) HIDE=$(HIDE) -C tex/ -B

pdf-clean:
	$(MAKE) -C tex/ clean

main-clean:
	rm -f *.map *.o $(BINLIST) core.* *.exe.stackdump *.pyc

clean: pdf-clean main-clean

distclean: clean
	rm -rf .depend
	rm -rf $(DISTUSER) $(DISTUSER).zip
	rm -rf .log
	rm -rf .profile

dist: pdf
	rm -rf $(DISTUSER) $(DISTUSER).zip
	mkdir -p $(DISTUSER)
	if [ -f tex/paper.pdf ] ; then  mv -f tex/paper.pdf $(DISTUSER)/ ; fi
	cp *.h *.cpp *.py *.sh README Makefile $(DISTUSER)/
	zip $(DISTUSER).zip $(DISTUSER)/*
