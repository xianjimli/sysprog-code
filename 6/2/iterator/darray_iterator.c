#include <string.h>
#include "darray_iterator.h"

typedef struct _PrivInfo
{
	DArray* darray;
	int offset;
}PrivInfo;

static Ret  darray_iterator_set(Iterator* thiz, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->darray != NULL, RET_INVALID_PARAMS);

	return darray_set_by_index(priv->darray, priv->offset, data);
}

static Ret  darray_iterator_get(Iterator* thiz, void** data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->darray != NULL && data != NULL, RET_INVALID_PARAMS);

	return darray_get_by_index(priv->darray, priv->offset, data);
}

static Ret  darray_iterator_next(Iterator* thiz)
{
	Ret ret = RET_FAIL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->darray != NULL, RET_INVALID_PARAMS);

	if((priv->offset + 1) < priv->darray->size)
	{
		priv->offset++;
		ret = RET_OK;
	}

	return ret;
}

static Ret  darray_iterator_prev(Iterator* thiz)
{
	Ret ret = RET_FAIL;
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->darray != NULL, RET_INVALID_PARAMS);

	if(priv->offset > 0)
	{
		priv->offset--;

		ret = RET_OK;
	}

	return RET_OK;
}

static Ret  darray_iterator_advance(Iterator* thiz, int offset)
{
	int new_offset = 0;
	Ret ret = RET_FAIL;
	PrivInfo*  priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->darray != NULL, RET_INVALID_PARAMS);

	new_offset = priv->offset + offset;
	if(new_offset >= 0 && new_offset < priv->darray->size)
	{
		priv->offset = new_offset;
	}

	return ret;
}

static int  darray_iterator_offset(Iterator* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->darray != NULL, RET_INVALID_PARAMS);

	return priv->offset;
}

static Ret  darray_iterator_clone(Iterator* thiz, Iterator** cloned)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;
	return_val_if_fail(priv->darray != NULL, RET_INVALID_PARAMS);

	*cloned = (Iterator*)malloc(sizeof(Iterator) + sizeof(PrivInfo));
	if(*cloned != NULL)
	{
		memcpy(*cloned, thiz, sizeof(Iterator) + sizeof(PrivInfo));
	}

	return *cloned != NULL ? RET_OK : RET_FAIL;
}

static void darray_iterator_destroy(Iterator* thiz)
{
	if(thiz != NULL)
	{
		free(thiz);
	}

	return;
}

Iterator* darray_iterator_create(DArray* darray)
{
	Iterator* thiz = NULL;
	return_val_if_fail(darray != NULL, NULL);

	if((thiz = malloc(sizeof(Iterator) + sizeof(PrivInfo))) != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;

		thiz->set      =  darray_iterator_set;
		thiz->get      =  darray_iterator_get;
		thiz->next     =  darray_iterator_next;
		thiz->prev     =  darray_iterator_prev;
		thiz->advance  =  darray_iterator_advance;
		thiz->clone    =  darray_iterator_clone;
		thiz->offset   =  darray_iterator_offset;
		thiz->destroy  =  darray_iterator_destroy;

		priv->darray  = darray;
		priv->offset = 0;
	}

	return thiz;
}

