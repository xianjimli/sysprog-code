#include "darray.h"

Ret  unique_print_int(void* ctx, void* data)
{
	if(*(int*)ctx != (int)data)
	{
		*(int*)ctx = (int)data;
		printf("%d ", (int)data);
	}

	return RET_OK;
}


#ifdef UNIQUE_TEST
#include "sort.h"
#include "darray.h"
#include <stdlib.h>
#include "test_helper.c"

int main(int argc, char* argv[])
{
	int i = 0;
	int n = 100;
	int data = 0;

	DArray* darray = darray_create(NULL, NULL);

	for(i = 0; i < n; i++)
	{
		darray_append(darray, (void*)(rand()%n));
	}

	darray_sort(darray, quick_sort, cmp_int);
	
	darray_foreach(darray, print_int, NULL);
	printf("\n");
	darray_get_by_index(darray, 0, (void*)&data);
	data = !data;
	darray_foreach(darray, unique_print_int, &data);
	printf("\n");
	darray_destroy(darray);

	return 0;
}

#endif/*UNIQUE_TEST*/

