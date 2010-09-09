BIN=simplePad
GCC=gcc -g -Wall
GTK=`pkg-config --cflags --libs gtk+-2.0`
GLADE=`pkg-config --cflags --libs libglade-2.0`

${BIN}: simplePad.o core.o buffers.o
	${GCC} -o ${BIN} core.o simplePad.o buffers.o ${GTK} ${GLADE}

core.o: core.c simplePad.h buffers.h
	${GCC} -c core.c ${GTK} ${GLADE}

simplePad.o: simplePad.c simplePad.h buffers.h
	${GCC} -c simplePad.c ${GTK} ${GLADE}

buffers.o: buffers.c buffers.h simplePad.h
	${GCC} -c buffers.c ${GTK} ${GLADE}

.PHONY: clean
clean:
	rm *.o ${BIN}