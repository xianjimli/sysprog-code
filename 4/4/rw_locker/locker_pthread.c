/*
 * File:    locker_pthread.c
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

#include <stdlib.h>
#include <pthread.h>
#include "locker_pthread.h"

typedef struct _PrivInfo
{
	pthread_mutex_t mutex;
}PrivInfo;

static Ret  locker_pthread_lock(Locker* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	int ret = pthread_mutex_lock(&priv->mutex);

	return ret == 0 ? RET_OK : RET_FAIL;
}

static Ret  locker_pthread_unlock(Locker* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	int ret = pthread_mutex_unlock(&priv->mutex);

	return ret == 0 ? RET_OK : RET_FAIL;
}

static void locker_pthread_destroy(Locker* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	int ret = pthread_mutex_destroy(&priv->mutex);

	SAFE_FREE(thiz);

	return;
}

Locker* locker_pthread_create(void)
{
	Locker* thiz = (Locker*)malloc(sizeof(Locker) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->lock    = locker_pthread_lock;
		thiz->unlock  = locker_pthread_unlock;
		thiz->destroy = locker_pthread_destroy;

		pthread_mutex_init(&(priv->mutex), NULL);
	}

	return thiz;
}


