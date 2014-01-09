/*
 * File:    dlist.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   double list header file.
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
 * 2008-12-10 Li XianJing <xianjimli@hotmail.com> add lock support.
 */

#include <stdio.h>
#include "rw_locker.h"

#ifndef DLIST_H
#define DLIST_H

DECLS_BEGIN

struct _DList;
typedef struct _DList DList;

typedef void     (*DListDataDestroyFunc)(void* ctx, void* data);
typedef int      (*DListDataCompareFunc)(void* ctx, void* data);
typedef Ret      (*DListDataVisitFunc)(void* ctx, void* data);

DList* dlist_create(DListDataDestroyFunc data_destroy, void* ctx, RwLocker* rw_locker);

Ret dlist_insert(DList* thiz, size_t index, void* data);
Ret dlist_prepend(DList* thiz, void* data);
Ret dlist_append(DList* thiz, void* data);
Ret dlist_delete(DList* thiz, size_t index);
Ret dlist_get_by_index(DList* thiz, size_t index, void** data);
Ret dlist_set_by_index(DList* thiz, size_t index, void* data);
size_t   dlist_length(DList* thiz);
int      dlist_find(DList* thiz, DListDataCompareFunc cmp, void* ctx);
Ret      dlist_foreach(DList* thiz, DListDataVisitFunc visit, void* ctx);

void dlist_destroy(DList* thiz);

DECLS_END

#endif/*DLIST*/

