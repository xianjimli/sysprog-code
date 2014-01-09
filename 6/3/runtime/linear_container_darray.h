/*
 * File:    linear_container_darray.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   darray implemented linear container.
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
 * 2009-03-01 Li XianJing <xianjimli@hotmail.com> created.
 *
 */

#ifndef LINEAR_CONTAINER_DARRAY_H
#define LINEAR_CONTAINER_DARRAY_H

#include "linear_container.h"

DECLS_BEGIN

LinearContainer* linear_container_darray_create(DataDestroyFunc data_destroy, void* ctx);

DECLS_END

#endif/*LINEAR_CONTAINER_DARRAY_H*/

