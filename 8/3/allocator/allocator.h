
#ifndef ALLOCATOR_H
#define ALLOCATOR_H

#include "typedef.h"

DECLS_BEGIN

struct  _Allocator;
typedef struct  _Allocator Allocator;

typedef void* (*AllocatorCallocFunc)(Allocator* thiz, size_t nmemb, size_t size);
typedef void* (*AllocatorAllocFunc)(Allocator* thiz, size_t size);
typedef void  (*AllocatorFreeFunc)(Allocator* thiz, void *ptr);
typedef void* (*AllocatorReallocFunc)(Allocator* thiz, void *ptr, size_t size);
typedef void  (*AllocatorDestroyFunc)(Allocator* thiz);

struct _Allocator
{
	AllocatorCallocFunc  calloc;
	AllocatorAllocFunc   alloc;
	AllocatorFreeFunc    free;
	AllocatorReallocFunc realloc;
	AllocatorDestroyFunc destroy;

	char priv[0];
};

static inline void* allocator_calloc(Allocator* thiz, size_t nmemb, size_t size)
{
	return_val_if_fail(thiz != NULL && thiz->calloc != NULL, NULL);

	return thiz->calloc(thiz, nmemb, size);
}

static inline void* allocator_alloc(Allocator* thiz, size_t size)
{
	return_val_if_fail(thiz != NULL && thiz->alloc != NULL, NULL);

	return thiz->alloc(thiz, size);
}

static inline void  allocator_free(Allocator* thiz, void *ptr)
{
	return_if_fail(thiz != NULL && thiz->free != NULL);

	thiz->free(thiz, ptr);

	return;
}

static inline void* allocator_realloc(Allocator* thiz, void *ptr, size_t size)
{
	return_val_if_fail(thiz != NULL && thiz->realloc != NULL, NULL);

	return thiz->realloc(thiz, ptr, size);
}

static inline void  allocator_destroy(Allocator* thiz)
{
	return_if_fail(thiz != NULL && thiz->destroy != NULL);

	return thiz->destroy(thiz);
}

DECLS_END

#endif/*ALLOCATOR_H*/

