include config.mk

SRC = ${wildcard src/*.c}
OBJ = ${SRC:.c=.o}

all: steelwm

build/%.o: src/%.c
	${CC} -c -g ${CFLAGS} $< -o $@

steelwm: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

clean:
	rm -f steelwm test build/*.o ${OBJ} steelwm-${VERSION}.tar.gz

dist: clean
	mkdir -p steelwm-${VERSION}
	cp -R COPYING Makefile README src/ steelwm-${VERSION}
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
