# Linux:
LIBS=-lvga -lm
OUT=test3
# DOS:
#LIBS=-lm
#OUT=test3.exe

default:
	gcc -Wall -g test3.c -o $(OUT) -I../include -L../lib -lwood -lair -lquick -lflash -lmetal -lcrash -lbubble -lheat -ldrwily $(LIBS)

clean:
	rm -f test3 test3.exe
