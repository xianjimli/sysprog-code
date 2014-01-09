all:
	gcc -Wall -g allocator_normal.c -DALLOCATOR_NORMAL_TEST -o allocator_normal_test
	gcc -Wall -g allocator_self_manage.c -DALLOCATOR_SELF_MANAGE_TEST \
		-o allocator_self_manage_test
	gcc -Wall -g allocator_self_manage.c allocator_shmem.c -DALLOCATOR_SHMEM_TEST\
		-o allocator_shmem_test
	gcc -Wall -g allocator_checkbo.c allocator_normal.c -DALLOCATOR_CHECKBO_TEST -o allocator_checkbo_test
clean:
	rm -f *test
