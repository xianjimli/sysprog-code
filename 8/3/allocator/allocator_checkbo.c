#include <string.h>
#include <assert.h>
#include "allocator_checkbo.h"

#define BEGIN_MAGIC 0xBb
#define END_MAGIC   0xEe

typedef struct _PrivInfo
{
	Allocator* real_allocator;
}PrivInfo;

static void*  allocator_checkbo_calloc(Allocator* thiz, size_t nmemb, size_t size)
{
	void* ptr = NULL;
	size_t total = nmemb * size;

	if((ptr = allocator_alloc(thiz, total)) != NULL)
	{
		memset(ptr, 0x00, total);
	}

	return ptr;
}

static void*  allocator_checkbo_alloc(Allocator* thiz, size_t size)
{
	char* ptr = NULL;
	size_t total = size + 3 * sizeof(void*);
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	if((ptr = allocator_alloc(priv->real_allocator, total)) != NULL)
	{
		*(size_t*)ptr = size;
		memset(ptr + sizeof(void*), BEGIN_MAGIC, sizeof(void*));
		memset(ptr + 2 * sizeof(void*) + size, END_MAGIC, sizeof(void*));
	}
	
	return ptr + 2 * sizeof(void*);
}

static void   allocator_checkbo_free(Allocator* thiz, void *ptr)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	if(ptr != NULL)
	{
		char magic[sizeof(void*)];
		char* real_ptr = (char*)ptr - 2 * sizeof(void*);
		size_t size = *(size_t*)(real_ptr);

		memset(magic, BEGIN_MAGIC, sizeof(void*));
		assert(memcmp((char*)ptr - sizeof(void*), magic, sizeof(void*)) == 0); 
		memset(magic, END_MAGIC, sizeof(void*));
		assert(memcmp((char*)ptr + size , magic, sizeof(void*)) == 0); 

		allocator_free(priv->real_allocator, real_ptr);
	}

	return;
}

static void*  allocator_checkbo_realloc(Allocator* thiz, void *ptr, size_t size)
{
	void* new_ptr = allocator_alloc(thiz, size);
	if(new_ptr != NULL)
	{
		allocator_free(thiz, ptr);
	}

	return new_ptr;
}

static void   allocator_checkbo_destroy(Allocator* thiz)
{
	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		allocator_destroy(priv->real_allocator);
		free(thiz);
	}

	return;
}

Allocator* allocator_checkbo_create(Allocator* real_allocator)
{
	Allocator* thiz = (Allocator*)calloc(1, sizeof(Allocator) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->calloc  = allocator_checkbo_calloc;
		thiz->alloc   = allocator_checkbo_alloc;
		thiz->realloc = allocator_checkbo_realloc;
		thiz->free    = allocator_checkbo_free;
		thiz->destroy = allocator_checkbo_destroy;

		priv->real_allocator = real_allocator;
	}

	return thiz;
}

#ifdef ALLOCATOR_CHECKBO_TEST
#include "allocator_normal.h"

int main(int argc, char* argv[])
{
	Allocator* real_allocator = allocator_normal_create();
	Allocator* allocator = allocator_checkbo_create(real_allocator);

	char* ptr = allocator_alloc(allocator, 10);
	memset(ptr, 0x00, 10);
	allocator_free(allocator, ptr);
	
	ptr = allocator_alloc(allocator, 10);
	memset(ptr, 0x00, 11);
	allocator_free(allocator, ptr);
	
	allocator_destroy(allocator);

	return 0;
}
#endif/*ALLOCATOR_CHECKBO_TEST*/

