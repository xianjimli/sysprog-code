all:
	gcc -g -shared dlist.c -o libdlist.so
	gcc -g -DDLIST_TEST dlist.c  -o dlist_test

clean:
	rm -f *test *.exe *.so
