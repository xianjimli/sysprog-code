/*
 * File:    atomic.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   atomic test
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

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "iatomic.h"

#ifdef ATOMIC_TEST

atomic_t g_count = {0};

static void* thread_inc(void* param)
{
	int i = 0;
	for(i = 0; i < 1000000; i++)
	{
		atomic_inc(&g_count);
	}

	return NULL;
}

static void* thread_dec(void* param)
{
	int i = 0;
	for(i = 0; i < 1000000; i++)
	{
		atomic_dec(&g_count);
	}

	return NULL;
}

int main(int argc, char* argv[])
{
	pthread_t inc_tid = 0;
	pthread_t dec_tid = 0;

	pthread_create(&inc_tid, NULL, thread_inc, NULL);
	pthread_create(&dec_tid, NULL, thread_dec, NULL);

	pthread_join(inc_tid, NULL);
	pthread_join(dec_tid, NULL);

	printf("count=%d\n", g_count.counter);

	return 0;
}
#endif/*ATOMIC_TEST*/
