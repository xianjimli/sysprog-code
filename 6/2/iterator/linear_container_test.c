#include <assert.h>
#include "linear_container.h"
#include "test_helper.c"
#include "linear_container_dlist.h"
#include "linear_container_darray.h"

static void test_int_linear_container(LinearContainer* linear_container)
{
	int i = 0;
	int n = 100;
	int data = 0;

	for(i = 0; i < n; i++)
	{
		assert(linear_container_append(linear_container, (void*)i) == RET_OK);
		assert(linear_container_length(linear_container) == (i + 1));
		assert(linear_container_get_by_index(linear_container, i, (void**)&data) == RET_OK);
		assert(data == i);
		assert(linear_container_set_by_index(linear_container, i, (void*)(2*i)) == RET_OK);
		assert(linear_container_get_by_index(linear_container, i, (void**)&data) == RET_OK);
		assert(data == 2*i);
		assert(linear_container_set_by_index(linear_container, i, (void*)i) == RET_OK);
		assert(linear_container_find(linear_container, cmp_int, (void*)i) == i);
	}

	for(i = 0; i < n; i++)
	{
		assert(linear_container_get_by_index(linear_container, 0, (void**)&data) == RET_OK);
		assert(data == (i));
		assert(linear_container_length(linear_container) == (n-i));
		assert(linear_container_delete(linear_container, 0) == RET_OK);
		assert(linear_container_length(linear_container) == (n-i-1));
		if((i + 1) < n)
		{
			assert(linear_container_get_by_index(linear_container, 0, (void**)&data) == RET_OK);
			assert((int)data == (i+1));
		}
	}
	
	assert(linear_container_length(linear_container) == 0);

	for(i = 0; i < n; i++)
	{
		assert(linear_container_prepend(linear_container, (void*)i) == RET_OK);
		assert(linear_container_length(linear_container) == (i + 1));
		assert(linear_container_get_by_index(linear_container, 0, (void**)&data) == RET_OK);
		assert(data == i);
		assert(linear_container_set_by_index(linear_container, 0, (void*)(2*i)) == RET_OK);
		assert(linear_container_get_by_index(linear_container, 0, (void**)&data) == RET_OK);
		assert(data == 2*i);
		assert(linear_container_set_by_index(linear_container, 0, (void*)i) == RET_OK);
	}

	i = n - 1;
	assert(linear_container_foreach(linear_container, check_and_dec_int, &i) == RET_OK);

	linear_container_destroy(linear_container);

	return;
}

static void test_invalid_params(void)
{
	printf("===========Warning is normal begin==============\n");
	assert(linear_container_length(NULL) == 0);
	assert(linear_container_prepend(NULL, 0) == RET_INVALID_PARAMS);
	assert(linear_container_append(NULL, 0) == RET_INVALID_PARAMS);
	assert(linear_container_delete(NULL, 0) == RET_INVALID_PARAMS);
	assert(linear_container_insert(NULL, 0, 0) == RET_INVALID_PARAMS);
	assert(linear_container_set_by_index(NULL, 0, 0) == RET_INVALID_PARAMS);
	assert(linear_container_get_by_index(NULL, 0, NULL) == RET_INVALID_PARAMS);
	assert(linear_container_find(NULL, NULL, NULL) < 0);
	assert(linear_container_foreach(NULL, NULL, NULL) == RET_INVALID_PARAMS);
	printf("===========Warning is normal end==============\n");

	return;
}

static void single_thread_test(void)
{
	LinearContainer* linear_container = linear_container_darray_create(NULL, NULL);
	test_int_linear_container(linear_container);
	linear_container = linear_container_dlist_create(NULL, NULL);
	test_int_linear_container(linear_container);
	test_invalid_params();

	return;
}

int main(int argc, char* argv[])
{
	single_thread_test();

	return 0;
}
