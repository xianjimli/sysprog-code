#include "allocator_pool.h"

typedef struct _FreeNode
{
	size_t length;
	struct _FreeNode* next;
	struct _FreeNode* prev;
}FreeNode;

#define MIN_SIZE 16
#define MAX_SIZE 64
#define EXPAND_DELTA 32

typedef struct _PrivInfo
{
	FreeNode* free_list[8];
	Allocator* real_allocator;
}PrivInfo;

static void*  allocator_pool_calloc(Allocator* thiz, size_t nmemb, size_t size)
{
	void* ptr = NULL;
	size_t total = nmemb * size;
	PrivInfo* priv = thiz->priv;

	if(total <= MAX_SIZE)
	{
		ptr = allocator_alloc(thiz, total);
		if(ptr != NULL)
		{
			memset(ptr, 0x00, total);
		}
	}
	else
	{
		ptr = allocator_calloc(priv->real_allocator, nmemb, size);
	}

	return ptr;
}

static void*  allocator_pool_alloc(Allocator* thiz, size_t size)
{
	void* ptr = NULL;
	PrivInfo* priv = thiz->priv;

	if(size <= MAX_SIZE)
	{
		size_t index = (size - 1)/8;
		if(priv->free_list[index] == NULL)
		{
			priv->free_list[index] = allocator_alloc(priv->real_allocator, 
				(index + 1) * 8
		}
	}
	else
	{
	}
}

static void   allocator_pool_free(Allocator* thiz, void *ptr)
{
	free(ptr);

	return;
}

static void*  allocator_pool_realloc(Allocator* thiz, void *ptr, size_t size)
{
	return realloc(ptr, size);
}

static void   allocator_pool_destroy(Allocator* thiz)
{
	if(thiz != NULL)
	{
		free(thiz);
	}

	return;
}

Allocator* allocator_pool_create(size_t expand_delta)
{
	Allocator* thiz = (Allocator*)calloc(1, sizeof(Allocator));

	if(thiz != NULL)
	{
		thiz->calloc  = allocator_pool_calloc;
		thiz->alloc   = allocator_pool_alloc;
		thiz->realloc = allocator_pool_realloc;
		thiz->free    = allocator_pool_free;
		thiz->destroy = allocator_pool_destroy;
	}

	return thiz;
}

#ifdef ALLOCATOR_POOL_TEST
int main(int argc, char* argv[])
{
	int i = 0;
	int n = 100;
	Allocator* allocator = allocator_pool_create(128);

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
#endif/*ALLOCATOR_POOL_TEST*/

