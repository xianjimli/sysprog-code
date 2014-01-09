/*
 * File:    queue.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   queue header file.
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

#include <stdio.h>
#include "typedef.h"

#ifndef QUEUE_H
#define QUEUE_H

DECLS_BEGIN

struct _Queue;
typedef struct _Queue Queue;

Queue* queue_create(DataDestroyFunc data_destroy, void* ctx);

Ret      queue_head(Queue* thiz, void** data);
Ret      queue_push(Queue* thiz, void* data);
Ret      queue_pop(Queue* thiz);
size_t   queue_length(Queue* thiz);
Ret      queue_foreach(Queue* thiz, DataVisitFunc visit, void* ctx);

void queue_destroy(Queue* thiz);

DECLS_END

#endif/*QUEUE_H*/

