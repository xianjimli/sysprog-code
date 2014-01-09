/*
 * File:    module_linux.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   dynamic module(linux implementation)
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

#include <dlfcn.h>
#include "module.h"

struct _Module
{
	void* handle;
};

Module* module_create(const char* file_name, ModuleFlags flags)
{
	Module* thiz = NULL;
	return_val_if_fail(file_name != NULL, NULL);
	
	if((thiz = malloc(sizeof(Module))) != NULL)
	{
		thiz->handle = dlopen(file_name, flags & MODULE_FLAGS_DELAY ? RTLD_LAZY : RTLD_NOW);
		if(thiz->handle == NULL)
		{
			free(thiz);
			thiz = NULL;
			printf("%s\n", dlerror());
		}
	}

	return thiz;
}

void*   module_sym(Module* thiz, const char* func_name)
{
	return_val_if_fail(thiz != NULL && thiz->handle != NULL && func_name != NULL, NULL);

	dlerror();
	return dlsym(thiz->handle, func_name);
}

void    module_destroy(Module* thiz)
{
	if(thiz != NULL)
	{
		if(thiz->handle != NULL)
		{
			dlclose(thiz->handle);
		}
		
		free(thiz);
	}
	
	return;
}

