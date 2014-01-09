/*
 * File:    module.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   dynamic module
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
 * 2009-03-16 Li XianJing <xianjimli@hotmail.com> created.
 *
 */

#ifndef MODULE_H
#define MODULE_H
#include "typedef.h"

DECLS_BEGIN

struct _Module;
typedef struct _Module Module;

typedef enum _ModuleFlags
{
	MODULE_FLAGS_NONE,
	MODULE_FLAGS_DELAY = 1
}ModuleFlags;

Module* module_create(const char* file_name, ModuleFlags flags);
void*   module_sym(Module* thiz, const char* func_name);
void    module_destroy(Module* thiz);

DECLS_END

#endif/*MODULE_H*/

