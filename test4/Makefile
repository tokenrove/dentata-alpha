
CFLAGS=-Wall -pedantic -g -I../include

ifeq ($(TARGET), dos)
SPRCOMP=..\utils\sprcomp
else
SPRCOMP=../utils/sprcomp
endif

default: test4$(OUT)

test4$(OUT): test4.o hao.spr ukyo.spr
	gcc $(CFLAGS) test4.o -o test4$(OUT) -L../lib -lair -lquick -lmetal -lcrash -lwood -lbubble -lflash -lheat -ldrwily $(LIBS)

%.spr: %.spd
	$(SPRCOMP) $<

clean:
	rm -f test4.exe test4
