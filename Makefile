#export OUT=
# Linux SVGAlib:
#export LIBS=-lvga -lm
#export TARGET=svgalib
# Linux X11:
#export LIBS=-L/usr/X11R6/lib -lXext -lX11 -lm
#export TARGET=x11
# DOS:
export LIBS=
export OUT=.exe
export TARGET=dos

default: tests

libs:
	make -C src

tests: libs
	make -C test1
	make -C test2
	make -C test3

clean:
	rm -f `find . -name "*~"` `find . -name "*.o"`

realclean: clean
	rm -f `find . -name "*.a"`
	make -C test1 clean
	make -C test2 clean
	make -C test3 clean
