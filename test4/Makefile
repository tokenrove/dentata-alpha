
CFLAGS=-Wall -pedantic -g -I../include

default: test4$(OUT)

test4$(OUT): test4.o
	gcc $(CFLAGS) test4.o -o test4$(OUT) -L../lib -lair -lquick -lmetal -lcrash -lwood -lbubble -lflash -lheat -ldrwily $(LIBS)

clean:
	rm -f test4.exe test4
