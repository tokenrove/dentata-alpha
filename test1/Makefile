
CFLAGS=-Wall -pedantic -g -I../include

default: test1$(OUT) test1-16a1$(OUT) test1-16a8$(OUT)

test1$(OUT): test1.o
	gcc $(CFLAGS) test1.o -o test1$(OUT) -L../lib -lair -lquick -lflash -lmetal -ldrwily $(LIBS)

test1-16a1$(OUT): test1-16a1.o
	gcc $(CFLAGS) test1-16a1.o -o test1-16a1$(OUT) -L../lib -lair -lquick -lflash -lmetal -ldrwily $(LIBS)

test1-16a8$(OUT): test1-16a8.o
	gcc $(CFLAGS) test1-16a8.o -o test1-16a8$(OUT) -L../lib -lair -lquick -lflash -lmetal -ldrwily $(LIBS)

clean:
	rm -f test1.exe test1 test1-16a1.exe test1-16a1
