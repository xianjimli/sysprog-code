#include <string.h>
#include "allocator_self_manage.h"

typedef struct _FreeNode
{
	size_t length;
	struct _FreeNode* next;
	struct _FreeNode* prev;
}FreeNode;

typedef struct _PrivInfo
{
	char*  buffer;
	size_t length;

	FreeNode* free_list;
}PrivInfo;

#define R8B(size) (((size+7) >> 3) << 3)
#define MIN_SIZE  R8B(sizeof(FreeNode))
#define REAL_SIZE(size) R8B((size > sizeof(FreeNode) ? size : MIN_SIZE) + sizeof(size_t));

static void*  allocator_self_manage_calloc(Allocator* thiz, size_t nmemb, size_t size)
{
	size_t length = nmemb * size;
	char* ptr = allocator_alloc(thiz, length);

	if(ptr != NULL)
	{
		memset(ptr, 0x00, length);
	}

	return ptr;
}

static void*  allocator_self_manage_alloc(Allocator* thiz, size_t size)
{
	FreeNode* iter = NULL;
	size_t length = REAL_SIZE(size);
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	/*查找第一个满足条件的空闲块*/
	for(iter = priv->free_list; iter != NULL; iter = iter->next)
	{
		if(iter->length > length)
		{
			break;
		}
	}
	
	return_val_if_fail(iter != NULL, NULL);
	
    /*如果找到的空闲块刚好满足需求，就从空闲块链表中移出它*/
	if(iter->length < (length + MIN_SIZE))
	{
		if(priv->free_list == iter)
		{
			priv->free_list = iter->next;
		}

		if(iter->prev != NULL)
		{
			iter->prev->next = iter->next;
		}
		if(iter->next != NULL)
		{
			iter->next->prev = iter->prev;
		}
	}
	else
	{
		/*如果找到的空闲块比较大，就把它拆成两个块，把多余的空闲内存放回去*/
		FreeNode* new_iter = (FreeNode*)((char*)iter + length);
		
		new_iter->length = iter->length - length;
		new_iter->next = iter->next;
		new_iter->prev = iter->prev;

		if(iter->prev != NULL)
		{
			iter->prev->next = new_iter;
		}
		if(iter->next != NULL)
		{
			iter->next->prev = new_iter;
		}

		if(priv->free_list == iter)
		{
			priv->free_list = new_iter;
		}
		iter->length = length;
	}
	/*返回可用的内存*/

	return (char*)iter + sizeof(size_t);
}

static void allocator_self_manage_merge2(Allocator* thiz, FreeNode* prev, FreeNode* next)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_if_fail(prev != NULL && next != NULL && prev->next == next);

	prev->next = next->next;
	if(next->next != NULL)
	{
		next->next->prev = prev;
	}
	prev->length += next->length;

	if(priv->free_list == next)
	{
		priv->free_list = prev;
	}

	return;
}

static void allocator_self_manage_merge(Allocator* thiz, FreeNode* iter)
{
	FreeNode* prev = iter->prev;
	FreeNode* next = iter->next;

	if(prev != NULL && ((size_t)prev + prev->length) == (size_t)iter)
	{
		allocator_self_manage_merge2(thiz, prev, iter);
		allocator_self_manage_merge(thiz, prev);
	}

	if(next != NULL && ((size_t)iter + iter->length) == (size_t)next)
	{
		allocator_self_manage_merge2(thiz, iter, next);
		allocator_self_manage_merge(thiz, iter);
	}

	return;
}

static void   allocator_self_manage_free(Allocator* thiz, void *ptr)
{
	FreeNode* iter = NULL;
	FreeNode* free_iter = NULL; 
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	
	return_if_fail(ptr != NULL);

	free_iter = (FreeNode*)((char*)ptr - sizeof(size_t));
	
	free_iter->prev = NULL;
	free_iter->next = NULL;

	if(priv->free_list == NULL)
	{
		priv->free_list = free_iter;

		return;
	}
	/*根据内存块地址的大小，把它插入到适当的位置。*/
	for(iter = priv->free_list; iter != NULL; iter = iter->next)
	{
		if((size_t)iter > (size_t)free_iter)
		{
			free_iter->next = iter;
			free_iter->prev = iter->prev;
			if(iter->prev != NULL)
			{
				iter->prev->next = free_iter;
			}
			iter->prev = free_iter;
			
			if(priv->free_list == iter)
			{
				priv->free_list = free_iter;
			}

			break;	
		}

		if(iter->next == NULL)
		{
			iter->next = free_iter;
			free_iter->prev = iter;

			break;
		}
	}

	/*对相邻居的内存进行合并*/
	allocator_self_manage_merge(thiz, free_iter);

	return;
}

static void*  allocator_self_manage_realloc(Allocator* thiz, void *ptr, size_t size)
{
	void* new_ptr = NULL;

	if(ptr != NULL)
	{
		size_t old_size = *(size_t*)((char*)ptr - sizeof(size_t)) - sizeof(size_t);
		if(old_size >= size && old_size <= (size + MIN_SIZE))
		{
			return ptr;
		}

		new_ptr = allocator_alloc(thiz, size);
		if(new_ptr != NULL)
		{
			memcpy(new_ptr, ptr, size < old_size ? size : old_size);
			allocator_free(thiz, ptr);
		}
	}
	else
	{
		new_ptr = allocator_alloc(thiz, size);
	}

	return new_ptr;
}

static void   allocator_self_manage_destroy(Allocator* thiz)
{
	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;
		priv->free_list = NULL;
		priv->buffer = NULL;

		free(thiz);
	}

	return;
}

Allocator* allocator_self_manage_create(void* buffer, size_t length)
{
	Allocator* thiz = NULL;
	
	return_val_if_fail(buffer != NULL && length > MIN_SIZE, NULL);

	thiz = (Allocator*)calloc(1, sizeof(Allocator)+sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->calloc  = allocator_self_manage_calloc;
		thiz->alloc   = allocator_self_manage_alloc;
		thiz->realloc = allocator_self_manage_realloc;
		thiz->free    = allocator_self_manage_free;
		thiz->destroy = allocator_self_manage_destroy;

		priv->buffer    = buffer;
		priv->length    = length;
		priv->free_list = (FreeNode*)buffer;
		priv->free_list->prev   = NULL;
		priv->free_list->next   = NULL;
		priv->free_list->length = length;

	}

	return thiz;
}

#ifdef ALLOCATOR_SELF_MANAGE_TEST

static void allocator_self_manage_dump(Allocator* thiz)
{
	int i = 0;
	FreeNode* iter = NULL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	for(iter = priv->free_list; iter != NULL; iter = iter->next, i++)
	{
		printf("[%d] %p %d\n", i, iter, iter->length);
	}

	return;
}
#include <assert.h>
static char buffer[1024 * 1024];
int main(int argc, char* argv[])
{
	int i = 0;
	int n = sizeof(buffer);
	char* ptrs[512] = {0};

	Allocator* allocator = allocator_self_manage_create(buffer, sizeof(buffer));

	assert(R8B(1) == 8);
	assert(R8B(3) == 8);
	assert(R8B(8) == 8);
	assert(R8B(9) == 16);
	assert(R8B(18) == 24);

	for(i = 0; i < n/3; i++)
	{
		char* ptr = allocator_alloc(allocator, i);
		allocator_free(allocator, ptr);
		ptr = allocator_calloc(allocator, 1, i);
		allocator_free(allocator, ptr);
	}
	
	for(i = 0; i < sizeof(ptrs)/sizeof(ptrs[0]); i++)
	{
		ptrs[i] = allocator_calloc(allocator, 1, i);
		//ptrs[i] = allocator_realloc(allocator, ptrs[i], 2*i);
	}

//	allocator_self_manage_dump(allocator);
	
	for(i = 0; i < sizeof(ptrs)/sizeof(ptrs[0]); i++)
	{
		allocator_free(allocator, ptrs[i]);
		ptrs[i] = NULL;
	}
	
	allocator_self_manage_dump(allocator);

#if 0
	for(i = 0; i < sizeof(ptrs)/sizeof(ptrs[0]); i++)
	{
		ptrs[i] = allocator_calloc(allocator, 1, i);
		ptrs[i] = allocator_realloc(allocator, ptrs[i], 2*i);
	}

//	allocator_self_manage_dump(allocator);
	
	for(i = 0; i < sizeof(ptrs)/sizeof(ptrs[0]); i++)
	{
		allocator_free(allocator, ptrs[i]);
		ptrs[i] = NULL;
	}
#endif
	allocator_self_manage_dump(allocator);
	allocator_destroy(allocator);

	return 0;
}
#endif/*ALLOCATOR_SELF_MANAGE_TEST*/

