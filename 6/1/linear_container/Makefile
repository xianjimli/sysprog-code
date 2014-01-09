all:
	gcc -Wall -g -DDARRAY_TEST darray.c -o darray_test
	gcc -Wall -g -DDLIST_TEST dlist.c -o dlist_test
	gcc -Wall -g -shared darray.c  dlist.c  linear_container_darray.c linear_container_dlist.c queue.c -o libcontainer.so
	gcc -Wall -g linear_container_test.c -L./ -lcontainer -o container_test
	gcc -Wall -g queue.c linear_container_dlist.c dlist.c -DQUEUE_TEST -o queue_test
clean:
	rm *test *.so
