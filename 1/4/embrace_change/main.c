/*
 * File:    main.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   demo how to print dlist.
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
 * 2008-11-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#include <stdio.h>
#include <assert.h>
#include "dlist.h"

static DListRet print_int(void* data)
{
	printf("%d ", (int)data);

	return DLIST_RET_OK;
}

int main(int argc, char* argv[])
{
	int i = 0;
	int n = 100;
	DList* dlist = dlist_create();

	for(i = 0; i < n; i++)
	{
		assert(dlist_append(dlist, (void*)i) == DLIST_RET_OK);
	}
	for(i = 0; i < n; i++)
	{
		assert(dlist_prepend(dlist, (void*)i) == DLIST_RET_OK);
	}

	dlist_print(dlist, print_int);

	dlist_destroy(dlist);

	return 0;
}

