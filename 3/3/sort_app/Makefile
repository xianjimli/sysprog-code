all:
	gcc -g -DDARRAY_TEST -lpthread darray.c sort.c -o darray_test
	gcc -g -Wall -DSORT_TEST sort.c -o sort_test
	gcc -g -Wall -DSEARCH_TEST search.c -o search_test
	gcc -g -Wall -DUNIQUE_TEST unique.c darray.c sort.c -o unique_test

clean:
	rm -f *test *.exe *.so
