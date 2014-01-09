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
#include "linear_container.h"

struct _Queue
{
	LinearContainer* linear_container;
};

Queue* queue_create(LinearContainer* container)
{
	Queue* thiz = NULL;
	
	return_val_if_fail(container != NULL, NULL);

	thiz = (Queue*)malloc(sizeof(Queue));

	if(thiz != NULL)
	{
		thiz->linear_container = container;
	}

	return thiz;
}

Ret      queue_head(Queue* thiz, void** data)
{
	return_val_if_fail(thiz != NULL && data != NULL, RET_INVALID_PARAMS);

	return linear_container_get_by_index(thiz->linear_container, 0, data);
}

Ret      queue_push(Queue* thiz, void* data)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	return linear_container_append(thiz->linear_container, data);
}

Ret      queue_pop(Queue* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	return linear_container_delete(thiz->linear_container, 0);
}

size_t   queue_length(Queue* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return linear_container_length(thiz->linear_container);	
}

Ret      queue_foreach(Queue* thiz, DataVisitFunc visit, void* ctx)
{
	return_val_if_fail(thiz != NULL && visit != NULL, RET_INVALID_PARAMS);

	return linear_container_foreach(thiz->linear_container, visit, ctx);
}

void queue_destroy(Queue* thiz)
{
	if(thiz != NULL)
	{
		linear_container_destroy(thiz->linear_container);
		thiz->linear_container = NULL;

		free(thiz);
	}

	return;
}

#ifdef QUEUE_TEST
#include "test_helper.c"
#include "linear_container_dlist.h"
int main(int argc, char* argv[])
{
	int i = 0;
	int n = 1000;
	int ret_data = 0;
	Queue* queue = queue_create(linear_container_dlist_create(NULL, NULL));		

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


