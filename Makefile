
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
