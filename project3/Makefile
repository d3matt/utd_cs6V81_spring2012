CC=gcc
CXX=g++
LD=$(CXX)
CFLAGS+=-Wall -g -fexceptions -O2
#uncomment to enable extra debug messages 3 is the most debugs, 1 is the least
#CFLAGS+=-DPROJ_DEBUG3
#CFLAGS+=-DPROJ_DEBUG2
#CFLAGS+=-DPROJ_DEBUG1
LDFLAGS=-lpthread -lrt

#pretty printing of CXX and LD
ifeq ($(HIDE),)
HIDE:= @
endif
MSG = $(if $(HIDE),echo,echo &> /dev/null)

#definable user
ifeq ($(DISTUSER),)
DISTUSER:=mjs010200
endif

PYTHON=$(shell ./findpython.sh)

BINLIST=testone testtwo testtwo.GC

COMMON_OBJECTS=$(addsuffix .o, LockStack LockFreeStack Exchanger Elimination TestCommon)
COMMON_OBJECTS_GC = $(addsuffix .GC, $(COMMON_OBJECTS)) GarbageCollector.o.GC


default: $(BINLIST)

# default compile rule
.cpp.o:
	@ mkdir -p .depend
	@ $(MSG) CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c -o $@ -MMD -MF $(@:%.o=.depend/%.d) $(firstword $^)

%.o.prof: %.cpp
	@ mkdir -p .depend
	@ $(MSG) CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c pg -o $@ -MMD -MF $(@:%.o.prof=.depend/$.d) $(firstword $^)

%.o.GC: %.cpp
	@ mkdir -p .depend
	@ $(MSG) CXX $@
	$(HIDE) $(CXX) $(CFLAGS) -c -D USE_GC -o $@ -MMD -MF $(@:%.o.prof=.depend/$.d) $(firstword $^)

#include generated dependency files
-include .depend/*.d

testone: testone.o $(COMMON_OBJECTS)
	@ $(MSG) LD $@
	$(HIDE) $(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^

testtwo: testtwo.o $(COMMON_OBJECTS)
	@ $(MSG) LD $@
	$(HIDE) $(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^

testtwo.GC: testtwo.o.GC $(COMMON_OBJECTS_GC)
	@ $(MSG) LD $@
	$(HIDE) $(LD) $(CFLAGS) $(LDFLAGS) -o $@ $^

tests test: $(BINLIST)
	$(PYTHON) ./test.py

.PHONY: tests test

pdf:
	@ $(MSG) PDF $(DISTUSER)
	$(HIDE) $(MAKE) HIDE=$(HIDE) -C tex/ -B

pdf-clean:
	$(MAKE) -C tex/ clean

main-clean:
	rm -f *.map *.o *.o.GC $(BINLIST) core.* *.exe.stackdump *.pyc *\~

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
