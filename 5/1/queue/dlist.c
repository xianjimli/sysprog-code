/*
 * File:    dlist.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   double list implementation.
 *
 * Copyright (c) Li XianJing
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2008-11-24 Li XianJing <xianjimli@hotmail.com> created
 * 2008-12-08 Li XianJing <xianjimli@hotmail.com> add autotest.
 *
 */

#include <stdlib.h>
#include "dlist.h"

typedef struct _DListNode
{
	struct _DListNode* prev;
	struct _DListNode* next;

	void* data;
}DListNode;

struct _DList
{
	DListNode* first;
	void* data_destroy_ctx;
	DataDestroyFunc data_destroy;
};


static void dlist_destroy_data(DList* thiz, void* data)
{
	if(thiz->data_destroy != NULL)
	{
		thiz->data_destroy(thiz->data_destroy_ctx, data);
	}

	return;
}

static DListNode* dlist_create_node(DList* thiz, void* data)
{
	DListNode* node = malloc(sizeof(DListNode));

	if(node != NULL)
	{
		node->prev = NULL;
		node->next = NULL;
		node->data = data;
	}

	return node;
}

static void dlist_destroy_node(DList* thiz, DListNode* node)
{
	if(node != NULL)
	{
		node->next = NULL;
		node->prev = NULL;
		dlist_destroy_data(thiz, node->data);
		SAFE_FREE(node);
	}

	return;
}

DList* dlist_create(DataDestroyFunc data_destroy, void* ctx)
{
	DList* thiz = malloc(sizeof(DList));

	if(thiz != NULL)
	{
		thiz->first  = NULL;
		thiz->data_destroy = data_destroy;
		thiz->data_destroy_ctx = ctx;
	}

	return thiz;
}

static DListNode* dlist_get_node(DList* thiz, size_t index, int fail_return_last)
{
	DListNode* iter = NULL;
	
	return_val_if_fail(thiz != NULL, NULL); 

	iter = thiz->first;

	while(iter != NULL && iter->next != NULL && index > 0)
	{
		iter = iter->next;
		index--;
	}

	if(!fail_return_last)
	{
		iter = index > 0 ? NULL : iter;
	}

	return iter;
}

Ret dlist_insert(DList* thiz, size_t index, void* data)
{
	Ret ret = RET_OK;
	DListNode* node = NULL;
	DListNode* cursor = NULL;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS); 


	do
	{
		if((node = dlist_create_node(thiz, data)) == NULL)
		{
			ret = RET_OOM;
			break;
		}

		if(thiz->first == NULL)
		{
			thiz->first = node;
			break;
		}

		cursor = dlist_get_node(thiz, index, 1);
		
		if(index < dlist_length(thiz))
		{
			node->next = cursor;
			if(cursor->prev != NULL)
			{
				cursor->prev->next = node;
			}
			cursor->prev = node;
			if(thiz->first == cursor)
			{
				thiz->first = node;
			}
		}
		else
		{
			cursor->next = node;
			node->prev = cursor;
		}
	}while(0);

	return ret;
}

Ret dlist_prepend(DList* thiz, void* data)
{
	return dlist_insert(thiz, 0, data);
}

Ret dlist_append(DList* thiz, void* data)
{
	return dlist_insert(thiz, -1, data);
}

Ret dlist_delete(DList* thiz, size_t index)
{
	Ret ret = RET_OK;
	DListNode* cursor = NULL;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS); 
	
	cursor = dlist_get_node(thiz, index, 0);

	do
	{
		if(cursor == NULL)
		{
			ret = RET_INVALID_PARAMS;
			break;
		}

		if(cursor != NULL)
		{
			if(cursor == thiz->first)
			{
				thiz->first = cursor->next;
			}

			if(cursor->next != NULL)
			{
				cursor->next->prev = cursor->prev;
			}

			if(cursor->prev != NULL)
			{
				cursor->prev->next = cursor->next;
			}

			dlist_destroy_node(thiz, cursor);
		}

	}while(0);

	return RET_OK;
}

Ret dlist_get_by_index(DList* thiz, size_t index, void** data)
{
	DListNode* cursor = NULL;

	return_val_if_fail(thiz != NULL && data != NULL, RET_INVALID_PARAMS); 

	cursor = dlist_get_node(thiz, index, 0);

	if(cursor != NULL)
	{
		*data = cursor->data;
	}

	return cursor != NULL ? RET_OK : RET_INVALID_PARAMS;
}

Ret dlist_set_by_index(DList* thiz, size_t index, void* data)
{
	DListNode* cursor = NULL;

	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	
	cursor = dlist_get_node(thiz, index, 0);

	if(cursor != NULL)
	{
		cursor->data = data;
	}

	return cursor != NULL ? RET_OK : RET_INVALID_PARAMS;
}

size_t dlist_length(DList* thiz)
{
	size_t length = 0;
	DListNode* iter = NULL;

	return_val_if_fail(thiz != NULL, 0);

	iter = thiz->first;

	while(iter != NULL)
	{
		length++;
		iter = iter->next;
	}

	return length;
}

Ret dlist_foreach(DList* thiz, DataVisitFunc visit, void* ctx)
{
	Ret ret = RET_OK;
	DListNode* iter = NULL;
	
	return_val_if_fail(thiz != NULL && visit != NULL, RET_INVALID_PARAMS);

	iter = thiz->first;

	while(iter != NULL && ret != RET_STOP)
	{
		ret = visit(ctx, iter->data);

		iter = iter->next;
	}

	return ret;
}

int      dlist_find(DList* thiz, DataCompareFunc cmp, void* ctx)
{
	int i = 0;
	DListNode* iter = NULL;

	return_val_if_fail(thiz != NULL && cmp != NULL, -1);

	iter = thiz->first;
	while(iter != NULL)
	{
		if(cmp(ctx, iter->data) == 0)
		{
			break;
		}
		i++;
		iter = iter->next;
	}

	return i;
}

void dlist_destroy(DList* thiz)
{
	DListNode* iter = NULL;
	DListNode* next = NULL;
	
	return_if_fail(thiz != NULL);

	iter = thiz->first;
	while(iter != NULL)
	{
		next = iter->next;
		dlist_destroy_node(thiz, iter);
		iter = next;
	}

	thiz->first = NULL;
	
	SAFE_FREE(thiz);

	return;
}

#ifdef DLIST_TEST

#include <assert.h>
#include <pthread.h>

static int cmp_int(void* ctx, void* data)
{
	return (int)data - (int)ctx;
}

static Ret print_int(void* ctx, void* data)
{
	printf("%d ", (int)data);

	return RET_OK;
}

static Ret check_and_dec_int(void* ctx, void* data)
{
	int* expected =(int*)ctx;
	assert(*expected == (int)data);

	(*expected)--;

	return RET_OK;
}

static void test_int_dlist(void)
{
	int i = 0;
	int n = 100;
	int data = 0;
	DList* dlist = dlist_create(NULL, NULL);

	for(i = 0; i < n; i++)
	{
		assert(dlist_append(dlist, (void*)i) == RET_OK);
		assert(dlist_length(dlist) == (i + 1));
		assert(dlist_get_by_index(dlist, i, (void**)&data) == RET_OK);
		assert(data == i);
		assert(dlist_set_by_index(dlist, i, (void*)(2*i)) == RET_OK);
		assert(dlist_get_by_index(dlist, i, (void**)&data) == RET_OK);
		assert(data == 2*i);
		assert(dlist_set_by_index(dlist, i, (void*)i) == RET_OK);
		assert(dlist_find(dlist, cmp_int, (void*)i) == i);
	}

	for(i = 0; i < n; i++)
	{
		assert(dlist_get_by_index(dlist, 0, (void**)&data) == RET_OK);
		assert(data == (i));
		assert(dlist_length(dlist) == (n-i));
		assert(dlist_delete(dlist, 0) == RET_OK);
		assert(dlist_length(dlist) == (n-i-1));
		if((i + 1) < n)
		{
			assert(dlist_get_by_index(dlist, 0, (void**)&data) == RET_OK);
			assert((int)data == (i+1));
		}
	}
	
	assert(dlist_length(dlist) == 0);

	for(i = 0; i < n; i++)
	{
		assert(dlist_prepend(dlist, (void*)i) == RET_OK);
		assert(dlist_length(dlist) == (i + 1));
		assert(dlist_get_by_index(dlist, 0, (void**)&data) == RET_OK);
		assert(data == i);
		assert(dlist_set_by_index(dlist, 0, (void*)(2*i)) == RET_OK);
		assert(dlist_get_by_index(dlist, 0, (void**)&data) == RET_OK);
		assert(data == 2*i);
		assert(dlist_set_by_index(dlist, 0, (void*)i) == RET_OK);
	}

	i = n - 1;
	assert(dlist_foreach(dlist, check_and_dec_int, &i) == RET_OK);

	dlist_destroy(dlist);

	return;
}

static void test_invalid_params(void)
{
	printf("===========Warning is normal begin==============\n");
	assert(dlist_length(NULL) == 0);
	assert(dlist_prepend(NULL, 0) == RET_INVALID_PARAMS);
	assert(dlist_append(NULL, 0) == RET_INVALID_PARAMS);
	assert(dlist_delete(NULL, 0) == RET_INVALID_PARAMS);
	assert(dlist_insert(NULL, 0, 0) == RET_INVALID_PARAMS);
	assert(dlist_set_by_index(NULL, 0, 0) == RET_INVALID_PARAMS);
	assert(dlist_get_by_index(NULL, 0, NULL) == RET_INVALID_PARAMS);
	assert(dlist_find(NULL, NULL, NULL) < 0);
	assert(dlist_foreach(NULL, NULL, NULL) == RET_INVALID_PARAMS);
	printf("===========Warning is normal end==============\n");

	return;
}

static void single_thread_test(void)
{
	test_int_dlist();
	test_invalid_params();

	return;
}

#define NR 1000
static void* producer(void* param)
{
	int i = 0;
	DList* dlist = (DList*)param;

	for(i = 0; i < NR; i++)
	{
		assert(dlist_append(dlist, (void*)i) == RET_OK);
	}
	sleep(1);
	for(i = 0; i < NR; i++)
	{
		assert(dlist_prepend(dlist, (void*)i) == RET_OK);
	}
	for(i = 0; i < NR; i++)
	{
		assert(dlist_insert(dlist, i, (void*)i) == RET_OK);
	}

	return NULL;
}

static void* consumer(void* param)
{
	int i = 0;
	DList* dlist = (DList*)param;

	for(i = 0; i < 3 * NR; i++)
	{
		usleep(20);
		assert(dlist_delete(dlist, 0) == RET_OK);
	}

	return NULL;
}

static void* reader(void* param)
{
	int i = 0;
	DList* dlist = (DList*)param;

	for(i = 0; i < NR; i++)
	{
		int length = dlist_length(dlist);
		dlist_find(dlist, cmp_int, (void*)i);
	}

	return NULL;
}

static void multi_thread_test(void)
{
	pthread_t consumer_tid = 0;
	pthread_t producer_tid = 0;
	pthread_t reader_tid = 0;
	DList* dlist = dlist_create(NULL, NULL);

	pthread_create(&producer_tid, NULL, producer, dlist);
	pthread_create(&consumer_tid, NULL, consumer, dlist);
	pthread_create(&reader_tid, NULL, reader, dlist);

	pthread_join(consumer_tid, NULL);
	pthread_join(producer_tid, NULL);
	pthread_join(reader_tid, NULL);
	printf("length=%d\n", dlist_length(dlist));
	dlist_destroy(dlist);

	return;
}
int main(int argc, char* argv[])
{
	single_thread_test();
	multi_thread_test();

	return 0;
}
#endif

