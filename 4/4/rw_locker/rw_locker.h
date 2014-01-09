/*
 * File:    rw_locker.h
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   read write locker
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
 * 2008-12-14 Li XianJing <xianjimli@hotmail.com> created.
 *
 */

#include "locker.h"

#ifndef RW_LOCKR_H
#define RW_LOCKER_H

struct _RwLocker;
typedef struct _RwLocker RwLocker;

RwLocker* rw_locker_create(Locker* rw_locker, Locker* rd_locker);

Ret rw_locker_wrlock(RwLocker* thiz);
Ret rw_locker_rdlock(RwLocker* thiz);
Ret rw_locker_unlock(RwLocker* thiz);

void rw_locker_destroy(RwLocker* thiz);

#endif/*RW_LOCKER_H*/

