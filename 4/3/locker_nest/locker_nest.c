/*
 * File:    locker_nest.c
 * Author:  Li XianJing <xianjimli@hotmail.com>
 * Brief:   nest locker.
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
#include "locker_nest.h"

typedef struct _PrivInfo
{
	int owner;
	int refcount;
	Locker* real_locker;
	TaskSelfFunc task_self;
}PrivInfo;

static Ret  locker_nest_lock(Locker* thiz)
{
	Ret ret = RET_OK;
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	if(priv->owner == priv->task_self())
	{
		priv->refcount++;
	}
	else
	{
		if( (ret = locker_lock(priv->real_locker)) == RET_OK)
		{
			priv->refcount = 1;
			priv->owner = priv->task_self();
		}
	}

	return ret;
}

static Ret  locker_nest_unlock(Locker* thiz)
{
	Ret ret = RET_OK;
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return_val_if_fail(priv->owner == priv->task_self(), RET_FAIL);
	
	priv->refcount--;
	if(priv->refcount == 0)
	{
		priv->owner = 0;
		ret = locker_unlock(priv->real_locker);
	}

	return ret;
}

static void  locker_nest_destroy(Locker* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	priv->owner = 0;
	priv->refcount = 0;
	locker_destroy(priv->real_locker);
	priv->real_locker = NULL;

	SAFE_FREE(thiz);

	return;
}

Locker* locker_nest_create(Locker* real_locker, TaskSelfFunc task_self)
{
	Locker* thiz = NULL;
	return_val_if_fail(real_locker != NULL && task_self != NULL, NULL);
	
	thiz = (Locker*)malloc(sizeof(Locker) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->lock    = locker_nest_lock;
		thiz->unlock  = locker_nest_unlock;
		thiz->destroy = locker_nest_destroy;

		priv->owner = 0;
		priv->refcount = 0;
		priv->real_locker = real_locker;
		priv->task_self = task_self;
	}

	return thiz;
}

