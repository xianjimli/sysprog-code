#include "linear_container.h"

Ret invert(LinearContainer* linear_container)
{
	int i = 0;
	int j = 0;
	void* data1 = NULL;
	void* data2 = NULL;

	return_val_if_fail(linear_container != NULL, RET_INVALID_PARAMS);

	j = linear_container_length(linear_container) - 1;
	for(; i < j; i++, j--)
	{
		linear_container_get_by_index(linear_container, i, &data1);
		linear_container_get_by_index(linear_container, j, &data2);
		linear_container_set_by_index(linear_container, i, data2);
		linear_container_set_by_index(linear_container, j, data1);
	}

	return RET_OK;
}

#ifdef INVERT_TEST
#include "test_helper.c"
#include "linear_container_dlist.h"

int main(int argc, char* argv[])
{
	int i = 0;
	int n = 101;

	LinearContainer* linear_container = linear_container_dlist_create(NULL, NULL);
	for(i = 0; i < n; i++)
	{
		linear_container_append(linear_container, (void*)i);
	}
	invert(linear_container);
	i--;
	linear_container_foreach(linear_container, check_and_dec_int, &i); 
	linear_container_destroy(linear_container);

	return 0;
}
#endif/*INVERT_TEST*/
