# First target is default target, if you just type:  make

FILE=cache.c

default: run

run: ${FILE}
	gcc -o cache.out ${FILE}
	./cache.out

gdb: cache
	gdb cache.out

emacs: ${FILE}
	emacs ${FILE}

vi: ${FILE}
	vi ${FILE}

clean:
	rm -f cache.out *~

dist:
	dir=`basename $$PWD`; cd ..; tar cvf $$dir.tar ./$$dir; gzip $$dir.tar
	dir=`basename $$PWD`; ls -l ../$$dir.tar.gz
