#include <string.h>
#include "dlist_iterator.h"

typedef struct _PrivInfo
{
	DList* dlist;
	DListNode* cursor;
	int offset;
}PrivInfo;

static Ret  dlist_iterator_set(Iterator* thiz, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->cursor != NULL && priv->dlist != NULL, RET_INVALID_PARAMS);

	priv->cursor->data = data;

	return RET_OK;
}

static Ret  dlist_iterator_get(Iterator* thiz, void** data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->cursor != NULL && priv->dlist != NULL && data != NULL, RET_INVALID_PARAMS);

	*data = priv->cursor->data;

	return RET_OK;
}

static Ret  dlist_iterator_next(Iterator* thiz)
{
	Ret ret = RET_FAIL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->cursor != NULL && priv->dlist != NULL, RET_INVALID_PARAMS);

	if(priv->cursor->next != NULL)
	{
		priv->cursor = priv->cursor->next;
		priv->offset++;

		ret = RET_OK;
	}

	return ret;
}

static Ret  dlist_iterator_prev(Iterator* thiz)
{
	Ret ret = RET_FAIL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->cursor != NULL && priv->dlist != NULL, RET_INVALID_PARAMS);

	if(priv->cursor->prev != NULL)
	{
		priv->cursor = priv->cursor->prev;
		priv->offset--;

		ret = RET_OK;
	}

	return RET_OK;
}

static Ret  dlist_iterator_advance(Iterator* thiz, int offset)
{
	int n   = offset;
	Ret ret = RET_FAIL;
	DListNode* iter = NULL;
	PrivInfo*  priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->cursor != NULL && priv->dlist != NULL, RET_INVALID_PARAMS);

	iter = priv->cursor;
	if(offset > 0)
	{
		for(n = offset; n > 0 && iter != NULL; n--)
		{
			iter = iter->next;
		}
	}
	else
	{
		for(n = -offset; n > 0 && iter != NULL; n--)
		{
			iter = iter->prev;
		}
	}

	if(iter != NULL)
	{
		priv->cursor = iter;
		priv->offset += offset;

		ret = RET_OK;
	}

	return ret;
}

static int  dlist_iterator_offset(Iterator* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->cursor != NULL && priv->dlist != NULL, RET_INVALID_PARAMS);

	return priv->offset;
}

static Ret  dlist_iterator_clone(Iterator* thiz, Iterator** cloned)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->cursor != NULL && priv->dlist != NULL, RET_INVALID_PARAMS);

	*cloned = (Iterator*)malloc(sizeof(Iterator) + sizeof(PrivInfo));
	if(*cloned != NULL)
	{
		memcpy(*cloned, thiz, sizeof(Iterator) + sizeof(PrivInfo));
	}

	return *cloned != NULL ? RET_OK : RET_FAIL;
}

static void dlist_iterator_destroy(Iterator* thiz)
{
	if(thiz != NULL)
	{
		free(thiz);
	}

	return;
}

Iterator* dlist_iterator_create(DList* dlist)
{
	Iterator* thiz = NULL;
	return_val_if_fail(dlist != NULL, NULL);

	if((thiz = malloc(sizeof(Iterator) + sizeof(PrivInfo))) != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->set      =  dlist_iterator_set;
		thiz->get      =  dlist_iterator_get;
		thiz->next     =  dlist_iterator_next;
		thiz->prev     =  dlist_iterator_prev;
		thiz->advance  =  dlist_iterator_advance;
		thiz->clone    =  dlist_iterator_clone;
		thiz->offset   =  dlist_iterator_offset;
		thiz->destroy  =  dlist_iterator_destroy;

		priv->dlist  = dlist;
		priv->cursor = dlist->first;
		priv->offset = 0;
	}

	return thiz;
}

