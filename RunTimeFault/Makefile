# Use "gmake" on BSD and "make" on Linux

NAME = RunTimeFault
VERSION = 0.0.1
TARNAME = ${NAME}-${VERSION}.tar.gz
BINS = ${NAME}
OBJS_MAIN = ${NAME}.o
OBJS_ALL = ${OBJS_MAIN}
PREFIX = /usr/local

all: ${BINS}

.SUFFIXES: .cxx .so

.cxx.o:
	g++ -Wall -pipe -DVERSION=${VERSION} -o $*.o $<

${NAME}: ${OBJS_MAIN}
	g++ -o $@ $^

#---------------------------------------------------------------------------
.PHONY: clean
clean:
	-rm -f ${OBJS_ALL} ${BINS} ${DLLS} .depend core

tgz: clean
	rm -f ${TARNAME}
	tar --exclude '*.o' -czf ${TARNAME} *

install: ${BINS}
	install -d         ${DESTDIR}${PREFIX}/bin
	install -s ${BINS} ${DESTDIR}${PREFIX}/bin

.depend:
	gcc -MM -MG *.c *.cxx >.depend

ifeq (.depend,$(wildcard .depend))
include .depend
endif

# vim: ft=make


