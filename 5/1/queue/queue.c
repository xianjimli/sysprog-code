/*
 * File:    queue.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   queue implementation.
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
 * 2008-02-14 Li XianJing <xianjimli@hotmail.com> created.
 */

#include "queue.h"
#include "dlist.h"

struct _Queue
{
	DList* dlist;
};

Queue* queue_create(DataDestroyFunc data_destroy, void* ctx)
{
	Queue* thiz = (Queue*)malloc(sizeof(Queue));

	if(thiz != NULL)
	{
		if((thiz->dlist = dlist_create(data_destroy, ctx)) == NULL)
		{
			free(thiz);
			thiz = NULL;
		}
	}

	return thiz;
}

Ret      queue_head(Queue* thiz, void** data)
{
	return_val_if_fail(thiz != NULL && data != NULL, RET_INVALID_PARAMS);

	return dlist_get_by_index(thiz->dlist, 0, data);
}

Ret      queue_push(Queue* thiz, void* data)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	return dlist_append(thiz->dlist, data);
}

Ret      queue_pop(Queue* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	return dlist_delete(thiz->dlist, 0);
}

size_t   queue_length(Queue* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return dlist_length(thiz->dlist);	
}

Ret      queue_foreach(Queue* thiz, DataVisitFunc visit, void* ctx)
{
	return_val_if_fail(thiz != NULL && visit != NULL, RET_INVALID_PARAMS);

	return dlist_foreach(thiz->dlist, visit, ctx);
}

void queue_destroy(Queue* thiz)
{
	if(thiz != NULL)
	{
		dlist_destroy(thiz->dlist);
		thiz->dlist = NULL;

		free(thiz);
	}

	return;
}

#ifdef QUEUE_TEST
#include "test_helper.c"
int main(int argc, char* argv[])
{
	int i = 0;
	int n = 1000;
	int ret_data = 0;
	Queue* queue = queue_create(NULL, NULL);		

	for(i = 0; i < n; i++)
	{
		assert(queue_push(queue, (void*)i) == RET_OK);
		assert(queue_head(queue, (void**)&ret_data) == RET_OK);
		assert(queue_length(queue) == (i+1));
	}

	queue_foreach(queue, print_int, NULL);

	for(i = 0; i < n; i++)
	{
		assert(queue_head(queue, (void**)&ret_data) == RET_OK);
		assert(ret_data == i );
		assert(queue_length(queue) == (n - i));
		assert(queue_pop(queue) == RET_OK);
	}

	queue_destroy(queue);
	return 0;
}
#endif/*QUEUE_TEST*/


