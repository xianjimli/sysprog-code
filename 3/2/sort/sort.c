/*
 * File:    sort.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   implementation of sort functions.
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
 * 2009-02-07 Li XianJing <xianjimli@hotmail.com> created.
 *
 */

#include "sort.h"

Ret bubble_sort(void** array, size_t nr, DataCompareFunc cmp)
{
	size_t i     = 0;
	size_t max   = 0;
	size_t right = 0;

	return_val_if_fail(array != NULL && cmp != NULL, RET_INVALID_PARAMS);

	if(nr < 2) 
	{
		return RET_OK;
	}

	for(right = nr - 1; right > 0; right--)
	{
		for(i = 1, max = 0; i < right; i++)
		{
			if(cmp(array[i], array[max]) > 0)
			{
				max = i;
			}
		}

		if(cmp(array[max], array[right]) > 0)
		{
			void* data = array[right];
			array[right] = array[max];
			array[max] = data;
		}
	}

	return RET_OK;
}

void quick_sort_impl(void** array, size_t left, size_t right, DataCompareFunc cmp)
{
	size_t save_left  = left;
	size_t save_right = right;
	void* x = array[left];

	while(left < right)
	{
		while(cmp(array[right], x) >= 0 && left < right) right--;
		if(left != right)
		{
			array[left] = array[right];
			left++;
		}

		while(cmp(array[left], x) <= 0 && left < right)	left++;
		if(left != right)
		{
			array[right] = array[left];
			right--;
		}
	}
	array[left] = x;

	if(save_left < left)
	{
		quick_sort_impl(array, save_left, left-1, cmp);
	}

	if(save_right > left)
	{
		quick_sort_impl(array, left+1, save_right, cmp);
	}

	return;
}

Ret quick_sort(void** array, size_t nr, DataCompareFunc cmp)
{
	Ret ret = RET_OK;

	return_val_if_fail(array != NULL && cmp != NULL, RET_INVALID_PARAMS);

	if(nr > 1)
	{
		quick_sort_impl(array, 0, nr - 1, cmp);
	}

	return ret;
}

static Ret merge_sort_impl(void** storage, void** array, size_t low, size_t mid, size_t high, DataCompareFunc cmp)
{
	size_t i = low;
	size_t j = low;
	size_t k = mid;

	if((low + 1) < mid)
	{
		size_t x = low + ((mid - low) >> 1);
		merge_sort_impl(storage, array, low, x, mid, cmp);
	}
	
	if((mid + 1) < high)
	{
		size_t x = mid + ((high - mid) >> 1);
		merge_sort_impl(storage, array, mid, x, high, cmp);
	}

	
	while(j < mid && k < high)
	{
		if(cmp(array[j], array[k]) <= 0)
		{
			storage[i++] = array[j++];
		}
		else
		{
			storage[i++] = array[k++];
		}
	}

	while(j < mid)
	{
		storage[i++] = array[j++];
	}

	while(k < high)
	{
		storage[i++] = array[k++];
	}

	for(i = low; i < high; i++)
	{
		array[i] = storage[i];
	}

	return RET_OK;
}

Ret merge_sort(void** array, size_t nr, DataCompareFunc cmp)
{
	void** storage = NULL;
	Ret ret = RET_OK;

	return_val_if_fail(array != NULL && cmp != NULL, RET_INVALID_PARAMS);

	if(nr > 1)
	{
		storage = (void**)malloc(sizeof(void*) * nr);
		if(storage != NULL)
		{
			ret = merge_sort_impl(storage, array, 0, nr>>1, nr, cmp);

			free(storage);
		}
	}

	return ret;
}


#ifdef SORT_TEST
#include <assert.h>
int int_cmp(void* a, void* b)
{
	return (int)a - (int)b;
}

int int_cmp_invert(void* a, void* b)
{
	return (int)b - (int)a;
}

static void** create_int_array(int n)
{
	int i = 0;
	int* array = (int*)malloc(sizeof(int) * n);

	for(i = 0; i < n; i++)
	{
		array[i] = rand();
	}

	return (void**)array;
}

static void sort_test_one_asc(SortFunc sort, int n)
{
	int i = 0;
	void** array = create_int_array(n);

	sort(array, n, int_cmp);

	for(i = 1; i < n; i++)
	{
		assert(array[i] >= array[i-1]);
	}

	free(array);

	return;
}

static void sort_test_one_dec(SortFunc sort, int n)
{
	int i = 0;
	void** array = create_int_array(n);

	sort((void**)array, n, int_cmp_invert);

	for(i = 1; i < n; i++)
	{
		assert(array[i] <= array[i-1]);
	}

	free(array);

	return;
}

static void sort_test(SortFunc sort)
{
	int i = 0;
	for(i = 0; i < 1000; i++)
	{
		sort_test_one_dec(sort, i);
		sort_test_one_asc(sort, i);
	}

	return ;
}

int main(int argc, char* argv[])
{
	srand(100);

	sort_test(quick_sort);
	sort_test(merge_sort);
	sort_test(bubble_sort);

	return 0;
}
#endif/*SORT_TEST*/
