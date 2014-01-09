/*
 * File:    stack.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   stack header file.
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

#ifndef STACK_H
#define STACK_H

DECLS_BEGIN

struct _Stack;
typedef struct _Stack Stack;

Stack* stack_create(DataDestroyFunc data_destroy, void* ctx);

Ret      stack_top(Stack* thiz, void** data);
Ret      stack_push(Stack* thiz, void* data);
Ret      stack_pop(Stack* thiz);
size_t   stack_length(Stack* thiz);
Ret      stack_foreach(Stack* thiz, DataVisitFunc visit, void* ctx);

void stack_destroy(Stack* thiz);

DECLS_END

#endif/*STACK_H*/

