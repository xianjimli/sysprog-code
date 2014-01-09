/*
 * File:    swmr_dlist.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   single writer multi reader lock-free dlist.
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
 * 2008-12-27 Li XianJing <xianjimli@hotmail.com> created.
 *
 */

#include "dlist.h"
#include "iatomic.h"

#define CAS(_a, _o, _n)                                    \
({ __typeof__(_o) __o = _o;                                \
   __asm__ __volatile__(                                   \
       "lock cmpxchg %3,%1"                                \
       : "=a" (__o), "=m" (*(volatile unsigned int *)(_a)) \
       :  "0" (__o), "r" (_n) );                           \
   __o;                                                    \
})

typedef struct _SwmrDList
{
	atomic_t rd_index_and_ref;
	DList* dlists[2];
}SwmrDList;

void swmr_dlist_destroy(SwmrDList* thiz);

SwmrDList* swmr_dlist_create(DListDataDestroyFunc data_destroy, void* ctx)
{
	Ret ret = RET_FAIL;
	SwmrDList* thiz = (SwmrDList*)calloc(1, sizeof(SwmrDList));

	do
	{
		if((thiz->dlists[0] = dlist_create(data_destroy, ctx, NULL)) == NULL)
		{
			break;
		}
	
		if((thiz->dlists[1] = dlist_create(data_destroy, ctx, NULL)) == NULL)
		{
			break;
		}

		ret = RET_OK;
	}while(0);

	if(ret != RET_OK)
	{
		swmr_dlist_destroy(thiz);
		thiz = NULL;
	}

	return thiz;
}

void swmr_dlist_destroy(SwmrDList* thiz)
{
	if(thiz != NULL)
	{
		if(thiz->dlists !=	NULL)
		{
			dlist_destroy(thiz->dlists[0]);
			dlist_destroy(thiz->dlists[1]);
			thiz->dlists[0] = NULL;
			thiz->dlists[1] = NULL;
		}
		free(thiz);
	}

	return;
}

int      swmr_dlist_find(SwmrDList* thiz, DListDataCompareFunc cmp, void* ctx)
{
	int ret = 0;
	return_val_if_fail(thiz != NULL && thiz->dlists != NULL, -1);

	atomic_inc(&(thiz->rd_index_and_ref));
	size_t rd_index = (thiz->rd_index_and_ref.counter>>24) & 0x1;
	ret = dlist_find(thiz->dlists[rd_index], cmp, ctx);
	atomic_dec(&(thiz->rd_index_and_ref));

	return ret;
}

int      swmr_dlist_length(SwmrDList* thiz)
{
	int ret = 0;
	return_val_if_fail(thiz != NULL && thiz->dlists != NULL, -1);

	atomic_inc(&(thiz->rd_index_and_ref));
	size_t rd_index = (thiz->rd_index_and_ref.counter>>24) & 0x1;
	ret = dlist_length(thiz->dlists[rd_index]);
	atomic_dec(&(thiz->rd_index_and_ref));

	return ret;
}

Ret swmr_dlist_insert(SwmrDList* thiz, size_t index, void* data)
{
	Ret ret = RET_FAIL;
	DList* wr_dlist = NULL;
	return_val_if_fail(thiz != NULL && thiz->dlists != NULL, ret);

	size_t wr_index = !((thiz->rd_index_and_ref.counter>>24) & 0x1);
	if((ret = dlist_insert(thiz->dlists[wr_index], index, data)) == RET_OK)
	{
		int rd_index_old = thiz->rd_index_and_ref.counter & 0xFF000000;
		int rd_index_new = wr_index << 24;

		do
		{
			usleep(100);
		}while(CAS(&(thiz->rd_index_and_ref), rd_index_old, rd_index_new));
		
		wr_index = rd_index_old>>24;
		ret = dlist_insert(thiz->dlists[wr_index], index, data);
	}

	return ret;
}

Ret swmr_dlist_delete(SwmrDList* thiz, size_t index)
{
	Ret ret = RET_FAIL;
	DList* wr_dlist = NULL;
	return_val_if_fail(thiz != NULL && thiz->dlists != NULL, ret);

	size_t wr_index = !((thiz->rd_index_and_ref.counter>>24) & 0x1);
	if((ret = dlist_delete(thiz->dlists[wr_index], index)) == RET_OK)
	{
		int rd_index_old = thiz->rd_index_and_ref.counter & 0xFF000000;
		int rd_index_new = wr_index << 24;

		do
		{
			usleep(100);
		}while(CAS(&(thiz->rd_index_and_ref), rd_index_old, rd_index_new));
		
		wr_index = rd_index_old>>24;
		ret = dlist_delete(thiz->dlists[wr_index], index);
	}

	return ret;
}

#ifdef SWMR_DLIST_TEST
#include <pthread.h>
#define NR 1000
static int cmp_int(void* ctx, void* data)
{
	return (int)data - (int)ctx;
}
static void* reader(void* param)
{
	int i = 0;
	int j = 0;
	size_t length = 0;
	SwmrDList* swmr = (SwmrDList*)param;

	for(j = 0; j < NR; j++)
	{
		for(i = 0; i < swmr_dlist_length(swmr); i++)
		{
			assert(swmr_dlist_find(swmr, cmp_int, (void*)i) == i);
		}
	}

	return NULL;
}

static void* writer(void* param)
{
	int i = 0;
	SwmrDList* swmr = (SwmrDList*)param;

	for(i = 0; i < NR; i++)
	{
		swmr_dlist_insert(swmr, i, (void*)i);
	}
	
	for(; i>0; i--)
	{
		swmr_dlist_delete(swmr, i);
	}

	return NULL;
}

#define RD_NR 100

int main(int argc, char* argv[])
{
	int i = 0;
	pthread_t wr_tid = 0;
	pthread_t rd_tids[RD_NR] = {0};
	SwmrDList* swmr = swmr_dlist_create(NULL, NULL);

	pthread_create(&wr_tid, NULL, writer, swmr);
	for(i = 0; i < RD_NR; i++)
	{
		pthread_create(rd_tids+i, NULL, reader, swmr);
	}
	
	for(i = 0; i < RD_NR; i++)
	{
		pthread_join(rd_tids[i], NULL);
	}

	pthread_join(wr_tid, NULL);

	return 0;
}
#endif/*SWMR_DLIST_TEST*/

