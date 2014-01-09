#include <sys/mman.h>
#include "allocator_shmem.h"
#include "allocator_self_manage.h"

typedef struct _PrivInfo
{
	char* buffer;
	size_t length;
	Allocator* manager;
}PrivInfo;

static void*  allocator_shmem_calloc(Allocator* thiz, size_t nmemb, size_t size)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return allocator_calloc(priv->manager, nmemb, size);
}

static void*  allocator_shmem_alloc(Allocator* thiz, size_t size)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	return allocator_alloc(priv->manager, size);
}

static void   allocator_shmem_free(Allocator* thiz, void *ptr)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return allocator_free(priv->manager, ptr);
}

static void*  allocator_shmem_realloc(Allocator* thiz, void *ptr, size_t size)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	return allocator_realloc(priv->manager, ptr, size);
}

static void   allocator_shmem_destroy(Allocator* thiz)
{
	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;
		munmap(priv->buffer, priv->length);
		free(thiz);
	}

	return;
}

Allocator* allocator_shmem_create(void* addr, size_t length)
{
	char* buffer = NULL;
	Allocator* thiz = NULL;
	
	buffer = mmap(addr, length, PROT_READ|PROT_WRITE, 
		MAP_SHARED|MAP_ANONYMOUS, -1, 0);

	return_val_if_fail(buffer != NULL, NULL);

	thiz = (Allocator*)calloc(1, sizeof(Allocator)+sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->calloc  = allocator_shmem_calloc;
		thiz->alloc   = allocator_shmem_alloc;
		thiz->realloc = allocator_shmem_realloc;
		thiz->free    = allocator_shmem_free;
		thiz->destroy = allocator_shmem_destroy;

		priv->buffer = buffer;
		priv->length = length;
		priv->manager = allocator_self_manage_create(buffer, length);
	}

	return thiz;
}

#ifdef ALLOCATOR_SHMEM_TEST
int main(int argc, char* argv[])
{
	int i = 0;
	int n = 1024;
	Allocator* allocator = allocator_shmem_create(0, n * 5);

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
#endif/*ALLOCATOR_SHMEM_TEST*/

