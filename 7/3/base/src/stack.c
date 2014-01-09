/*
 * File:    stack.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   stack implementation.
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

#include "stack.h"
#include "dlist.h"

struct _Stack
{
	DList* dlist;
};

Stack* stack_create(DataDestroyFunc data_destroy, void* ctx)
{
	Stack* thiz = (Stack*)malloc(sizeof(Stack));

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

Ret      stack_top(Stack* thiz, void** data)
{
	return_val_if_fail(thiz != NULL && data != NULL, RET_INVALID_PARAMS);

	return dlist_get_by_index(thiz->dlist, 0, data);
}

Ret      stack_push(Stack* thiz, void* data)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	return dlist_prepend(thiz->dlist, data);
}

Ret      stack_pop(Stack* thiz)
{
	return_val_if_fail(thiz != NULL, RET_INVALID_PARAMS);

	return dlist_delete(thiz->dlist, 0);
}

size_t   stack_length(Stack* thiz)
{
	return_val_if_fail(thiz != NULL, 0);

	return dlist_length(thiz->dlist);	
}

Ret      stack_foreach(Stack* thiz, DataVisitFunc visit, void* ctx)
{
	return_val_if_fail(thiz != NULL && visit != NULL, RET_INVALID_PARAMS);

	return dlist_foreach(thiz->dlist, visit, ctx);
}

void stack_destroy(Stack* thiz)
{
	if(thiz != NULL)
	{
		dlist_destroy(thiz->dlist);
		thiz->dlist = NULL;

		free(thiz);
	}

	return;
}

#ifdef STACK_TEST

#include "test_helper.c"
int main(int argc, char* argv[])
{
	int i = 0;
	int n = 1000;
	int ret_data = 0;
	Stack* stack = stack_create(NULL, NULL);		

	for(i = 0; i < n; i++)
	{
		assert(stack_push(stack, (void*)i) == RET_OK);
		assert(stack_top(stack, (void**)&ret_data) == RET_OK);
		assert(stack_length(stack) == (i+1));
	}

	stack_foreach(stack, print_int, NULL);

	for(i = 0; i < n; i++)
	{
		assert(stack_top(stack, (void**)&ret_data) == RET_OK);
		assert(ret_data == (n - i - 1));
		assert(stack_length(stack) == (n - i));
		assert(stack_pop(stack) == RET_OK);
	}

	stack_destroy(stack);

	return 0;
}
#endif/*STACK_TEST*/


