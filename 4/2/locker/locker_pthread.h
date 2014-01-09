/*
 * File:    locker_pthread.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   pthread mutex implemented locker.
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
 * 2008-12-10 Li XianJing <xianjimli@hotmail.com> created.
 *
 */
#include "locker.h"

#ifndef LOCKER_PTHREAD_H
#define LOCKER_PTHREAD_H

DECLS_BEGIN

Locker* locker_pthread_create(void);

DECLS_END

#endif/*LOCKER_PTHREAD_H*/

