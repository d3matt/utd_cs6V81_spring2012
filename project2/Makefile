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

BINLIST=simple_test second_test

COMMON_OBJECTS = TASLock.o TTASLock.o BackoffLock.o ALock.o test_common.o
COMMON_OBJECTS_PROF = $(addsuffix .prof, $(COMMON_OBJECTS))

default: $(BINLIST)

# default compile rule
.cpp.o:
	@ mkdir -p .depend
	@ echo CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c -o $@ -MMD -MF $(@:%.o=.depend/%.d) $(firstword $^)

%.o.prof: %.cpp
	@ mkdir -p .depend
	@ echo CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c -pg -o $@ -MMD -MF $(@:%.o.prof=.depend/$.d) $(firstword $^)

#include generated dependency files
-include .depend/*.d

simple_test: simple_test.o $(COMMON_OBJECTS)
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -o $@ $^

second_test: second_test.o $(COMMON_OBJECTS)
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -o $@ $^

simple_test.prof: simple_test.o.prof $(COMMON_OBJECTS_PROF)
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -pg -o $@ $^

second_test.prof: second_test.o.prof $(COMMON_OBJECTS_PROF)
	@ echo LD $@
	$(HIDE) $(LD) $(LDFLAGS) -pg -o $@ $^

prof : simple_test.prof second_test.prof

tests test: second_test
	$(PYTHON) ./test.py

.PHONY: prof tests test

test-prof : prof
	@echo PROF simple_test
	$(HIDE) mkdir -p .profile/
	$(HIDE) ./test_prof.py

pdf:
	@echo PDF $(DISTUSER)
	$(HIDE) $(MAKE) HIDE=$(HIDE) -C tex/

clean:
	rm -f *.map *.o *.o.prof $(BINLIST) $(addsuffix .prof, $(BINLIST)) core.* *.exe.stackdump *.pyc

distclean: clean
	rm -rf .depend
	rm -rf $(DISTUSER) $(DISTUSER).zip
	rm -rf .log
	rm -rf .profile

dist: pdf
	rm -rf $(DISTUSER) $(DISTUSER).zip
	mkdir -p $(DISTUSER)
	if [ -f tex/paper.pdf ] ; then  mv -f tex/paper.pdf $(DISTUSER)/ ; fi
	cp *.h *.cpp README Makefile $(DISTUSER)/
	zip $(DISTUSER).zip $(DISTUSER)/*
