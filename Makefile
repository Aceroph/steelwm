include config.mk

SRC = steelwm.c steelwm.c util.c
OBJ = ${SRC:.c=.o}

all: steelwm

.c.o:
	${CC} -c ${CFLAGS} $<

${OBJ}: config.h config.mk

config.h:
	cp config.def.h $@

steelwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f steelwm ${OBJ} steelwm-${VERSION}.tar.gz

dist: clean
	mkdir -p steelwm-${VERSION}
	cp -R LICENSE Makefile README config.def.h config.mk\
		steelwm.1 drw.h util.h ${SRC} transient.c steelwm-${VERSION}
	tar -cf steelwm-${VERSION}.tar steelwm-${VERSION}
	gzip steelwm-${VERSION}.tar
	rm -rf steelwm-${VERSION}

install: all
	mkdir -p ${DESTDIR}${PREFIX}/bin
	cp -f steelwm ${DESTDIR}${PREFIX}/bin
	chmod 755 ${DESTDIR}${PREFIX}/bin/steelwm
	mkdir -p ${DESTDIR}${MANPREFIX}/man1
	sed "s/VERSION/${VERSION}/g" < steelwm.1 > ${DESTDIR}${MANPREFIX}/man1/steelwm.1
	chmod 644 ${DESTDIR}${MANPREFIX}/man1/steelwm.1

uninstall:
	rm -f ${DESTDIR}${PREFIX}/bin/steelwm\
		${DESTDIR}${MANPREFIX}/man1/steelwm.1

.PHONY: all clean dist install uninstall
