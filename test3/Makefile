
CFLAGS=-Wall -pedantic -g -I../include

ifeq ($(TARGET), dos)
SPRCOMP=..\utils\sprcomp
else
SPRCOMP=../utils/sprcomp
endif

ifeq ($(TARGET), dos)
TMCOMP=..\utils\tmcomp
else
TMCOMP=../utils/tmcomp
endif

default: test3$(OUT)

test3$(OUT): test3.o hero.spr head.spr world00.map
	gcc $(CFLAGS) test3.o -o test3$(OUT) -L../lib -lair -lquick -lmetal -lcrash -lwood -lbubble -lheat -lflash -ldrwily $(LIBS)

%.spr: %.spd
	$(SPRCOMP) $<

%.map: %.spd
	$(TMCOMP) $<

clean:
	rm -f test3.exe test3
