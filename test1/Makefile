# Linux:
LIBS=-lvga -lm
OUT=test1
# DOS:
#LIBS=
#OUT=test1.exe

default:
	gcc -Wall -g test1.c -o $(OUT) -I../include -L../lib -lair -lquick -lflash -lmetal -ldrwily $(LIBS)

clean:
	rm -f test1.exe test1
