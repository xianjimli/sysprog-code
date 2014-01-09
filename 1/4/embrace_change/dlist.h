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
 * 2008-11-09 Li XianJing <xianjimli@hotmail.com> created
 *
 */

#ifndef DLIST_H
#define DLIST_H

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

typedef enum _DListRet
{
	DLIST_RET_OK,
	DLIST_RET_OOM,
	DLIST_RET_STOP,
	DLIST_RET_PARAMS,
	DLIST_RET_FAIL
}DListRet;

struct _DList;
typedef struct _DList DList;

typedef DListRet (*DListDataPrintFunc)(void* data);

DList* dlist_create(void);

DListRet dlist_insert(DList* thiz, size_t index, void* data);
DListRet dlist_prepend(DList* thiz, void* data);
DListRet dlist_append(DList* thiz, void* data);
DListRet dlist_delete(DList* thiz, size_t index);
DListRet dlist_get_by_index(DList* thiz, size_t index, void** data);
DListRet dlist_set_by_index(DList* thiz, size_t index, void* data);
size_t   dlist_length(DList* thiz);
DListRet dlist_print(DList* thiz, DListDataPrintFunc print);

void dlist_destroy(DList* thiz);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif/*DLIST*/

