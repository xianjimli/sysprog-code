#include "allocator_normal.h"

static void*  allocator_normal_calloc(Allocator* thiz, size_t nmemb, size_t size)
{
	return calloc(nmemb, size);
}

static void*  allocator_normal_alloc(Allocator* thiz, size_t size)
{
	return malloc(size);
}

static void   allocator_normal_free(Allocator* thiz, void *ptr)
{
	free(ptr);

	return;
}

static void*  allocator_normal_realloc(Allocator* thiz, void *ptr, size_t size)
{
	return realloc(ptr, size);
}

static void   allocator_normal_destroy(Allocator* thiz)
{
	if(thiz != NULL)
	{
		free(thiz);
	}

	return;
}

Allocator* allocator_normal_create(void)
{
	Allocator* thiz = (Allocator*)calloc(1, sizeof(Allocator));

	if(thiz != NULL)
	{
		thiz->calloc  = allocator_normal_calloc;
		thiz->alloc   = allocator_normal_alloc;
		thiz->realloc = allocator_normal_realloc;
		thiz->free    = allocator_normal_free;
		thiz->destroy = allocator_normal_destroy;
	}

	return thiz;
}

#ifdef ALLOCATOR_NORMAL_TEST
int main(int argc, char* argv[])
{
	int i = 0;
	int n = 100;
	Allocator* allocator = allocator_normal_create();

	for(i = 0; i < n; i++)
	{
		char* ptr = allocator_alloc(allocator, i);
		ptr = allocator_realloc(allocator, ptr, i+4);
		allocator_free(allocator, ptr);
		ptr = allocator_calloc(allocator, i+4, 4);
		allocator_free(allocator, ptr);
	}
	
	allocator_destroy(allocator);

	return 0;
}
#endif/*ALLOCATOR_NORMAL_TEST*/

