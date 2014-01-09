all:
	gcc -g atomic.c -lpthread -DATOMIC_TEST -o atomic_test
	gcc -g fifo_ring.c -lpthread -DFIFO_RING_TEST -o fifo_ring_test
	gcc -g swmr_dlist.c dlist.c -lpthread -DSWMR_DLIST_TEST -o swmr_dlist_test
clean:
	rm -f *test

