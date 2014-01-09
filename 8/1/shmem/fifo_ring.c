/*
 * File:    fifo_ring.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   fifo ring
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

#include "typedef.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct _FifoRing
{
	int inited;
	int r_cursor;
	int w_cursor;
	size_t length;
	void* data[0];
	
}FifoRing;

static int g_shmem_fd = -1;
void* shmem_alloc(size_t size)
{
	g_shmem_fd = open("/tmp/shmem_demo", O_RDWR);

	if(g_shmem_fd < 0)
	{
		char* buffer = calloc(size, 1);
		g_shmem_fd = open("/tmp/shmem_demo", O_RDWR | O_CREAT, 0640);
		write(g_shmem_fd, buffer, size);
		free(buffer);
	}

	void* addr =  mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, g_shmem_fd, 0); 
	
	return addr;
}

void shmem_free(void* addr, size_t size)
{
	munmap(addr, size);
	close(g_shmem_fd);

	return;
}

FifoRing* fifo_ring_create(size_t length)
{
	FifoRing* thiz = NULL;
	
	return_val_if_fail(length > 1, NULL);

	thiz = (FifoRing*)shmem_alloc(sizeof(FifoRing) + length * sizeof(void*));

	if(thiz != NULL)
	{
		if(thiz->inited == 0)
		{
			thiz->r_cursor = 0;
			thiz->w_cursor = 0;
			thiz->length   = length;
			thiz->inited = 1;
		}
	}

	return thiz;
}

Ret fifo_ring_pop(FifoRing* thiz, void** data)
{
	Ret ret = RET_FAIL;
	return_val_if_fail(thiz != NULL && data != NULL, RET_FAIL);

	if(thiz->r_cursor != thiz->w_cursor)
	{
		*data = thiz->data[thiz->r_cursor];
		thiz->r_cursor = (thiz->r_cursor + 1)%thiz->length;

		ret = RET_OK;
	}

	return ret;
}

Ret fifo_ring_push(FifoRing* thiz, void* data)    
{
	int w_cursor = 0;
	Ret ret = RET_FAIL;
	return_val_if_fail(thiz != NULL, RET_FAIL);

	w_cursor = (thiz->w_cursor + 1) % thiz->length;
	
	if(w_cursor != thiz->r_cursor)
	{
		thiz->data[thiz->w_cursor] = data;
		thiz->w_cursor = w_cursor;

		ret = RET_OK;
	}

	return ret;
}

void fifo_ring_destroy(FifoRing* thiz)
{
	if(thiz != NULL)
	{
		shmem_free(thiz, sizeof(FifoRing) + thiz->length * sizeof(void*));
	}

	return;
}

#ifdef FIFO_RING_TEST
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NR 10

static void push(FifoRing* fifo)
{
	int i = 0;

	while(i < NR)
	{
		if(fifo_ring_push(fifo, (void*)i) == RET_OK)
		{
			printf("%s: %d\n", __func__, i);
			i++;
		}
		else
		{
			usleep(100);
		}
	}

	return;
}

static void pop(FifoRing* fifo)
{
	int i = 0;
	void* data = 0;

	while(i < NR)
	{
		if(fifo_ring_pop(fifo, (void**)&data) == RET_OK)
		{
			printf("%s: %d data=%d\n", __func__, i, data);
			assert(i == (int)data);
			i++;
		}
		else
		{
			usleep(100);
		}
	}

	return ;
}

int main(int argc, char* argv[])
{
	if(argc != 2)
	{
		printf("usage: %s pop|push\n", argv[0]);
		return 0;
	}

	FifoRing* fifo = fifo_ring_create(1000);
	if(strcmp(argv[1], "pop") == 0)
	{
		pop(fifo);
	}
	else
	{
		push(fifo);
	}
	fifo_ring_destroy(fifo);

	return 0;
}

#endif/*FIFO_RING_TEST*/

