/*
 * File:    darray.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   dynamic array implementation.
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
 * 2009-01-02 Li XianJing <xianjimli@hotmail.com> created
 */

#include <stdio.h>
#include "typedef.h"

#ifndef DARRAY_H
#define DARRAY_H

DECLS_BEGIN

struct _DArray;
typedef struct _DArray DArray;

DArray* darray_create(DataDestroyFunc data_destroy, void* ctx);

Ret    darray_insert(DArray* thiz, size_t index, void* data);
Ret    darray_prepend(DArray* thiz, void* data);
Ret    darray_append(DArray* thiz, void* data);
Ret    darray_delete(DArray* thiz, size_t index);
Ret    darray_get_by_index(DArray* thiz, size_t index, void** data);
Ret    darray_set_by_index(DArray* thiz, size_t index, void* data);
size_t darray_length(DArray* thiz);
int    darray_find(DArray* thiz, DataCompareFunc cmp, void* ctx);
Ret    darray_foreach(DArray* thiz, DataVisitFunc visit, void* ctx);

void darray_destroy(DArray* thiz);

DECLS_END

#endif/*DARRAY_H*/

