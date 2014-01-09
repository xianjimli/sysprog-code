lib_LTLIBRARIES=libbase.la
libbase_la_SOURCES= darray.c \
	darray.h \
	dlist.c \
	dlist.h \
	darray_iterator.h \
	dlist_iterator.h \
	hash_table.c \
	hash_table.h \
	invert.c \
	iterator.h \
	linear_container_darray.c \
	linear_container_darray.h \
	linear_container_dlist.c \
	linear_container_dlist.h \
	linear_container.h \
	queue.c \
	queue.h \
	sort.c \
	sort.h \
	stack.c \
	stack.h \
	typedef.h 

libbase_la_LDFLAGS=-lpthread

noinst_PROGRAMS=darray_test dlist_test

darray_test_SOURCES=darray.c
darray_test_CFLAGS=-DDARRAY_TEST

dlist_test_SOURCES=dlist.c
dlist_test_CFLAGS=-DDLIST_TEST

basedir=$(includedir)/base
base_HEADERS=darray.h dlist.h iterator.h linear_container_dlist.h typedef.h \
	darray_iterator.h  dlist_iterator.h  linear_container_darray.h  \
	linear_container.h

EXTRA_DIST=\
	linear_container_test.c \
	invert_ng.c \
	darray_iterator.c \
	dlist_iterator.c \
	test_helper.c 
