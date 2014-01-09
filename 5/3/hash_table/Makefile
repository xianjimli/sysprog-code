CFILES=dlist.c  
all:
	gcc -g -shared -lpthread $(CFILES) -o libdlist.so
	gcc -g -DDLIST_TEST -lpthread $(CFILES)  -o dlist_test
	gcc -g -DHASH_TABLE_TEST hash_table.c dlist.c -o hash_table_test

clean:
	rm -f *test *.exe *.so
