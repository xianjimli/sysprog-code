#include "darray.h"
#include "linear_container.h"

typedef struct _PrivInfo
{
	DArray* darray;
}PrivInfo;

static Ret linear_container_darray_insert(LinearContainer* thiz, size_t index, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_insert(priv->darray, index, data);
}

static Ret linear_container_darray_prepend(LinearContainer* thiz, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_prepend(priv->darray, data);
}

static Ret linear_container_darray_append(LinearContainer* thiz, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_append(priv->darray, data);
}

static Ret linear_container_darray_delete(LinearContainer* thiz, size_t index)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_delete(priv->darray, index);
}

static Ret linear_container_darray_get_by_index(LinearContainer* thiz, size_t index, void** data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_get_by_index(priv->darray, index, data);
}

static Ret linear_container_darray_set_by_index(LinearContainer* thiz, size_t index, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_set_by_index(priv->darray, index, data);
}

static size_t linear_container_darray_length(LinearContainer* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_length(priv->darray);
}

static int    linear_container_darray_find(LinearContainer* thiz, DataCompareFunc cmp, void* ctx)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_find(priv->darray, cmp, ctx);
}

static Ret    linear_container_darray_foreach(LinearContainer* thiz, DataVisitFunc visit, void* ctx)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return darray_foreach(priv->darray, visit, ctx);
}

static void   linear_container_darray_destroy(LinearContainer* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	darray_destroy(priv->darray);
	free(thiz);

	return;
}

LinearContainer* linear_container_darray_create(DataDestroyFunc data_destroy, void* ctx)
{
	LinearContainer* thiz = (LinearContainer*)malloc(sizeof(LinearContainer) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;
		priv->darray = darray_create(data_destroy, ctx);

		thiz->insert        =  linear_container_darray_insert;
		thiz->prepend       =  linear_container_darray_prepend;
		thiz->append        =  linear_container_darray_append;
		thiz->del           =  linear_container_darray_delete;
		thiz->get_by_index  =  linear_container_darray_get_by_index;
		thiz->set_by_index  =  linear_container_darray_set_by_index;
		thiz->length        =  linear_container_darray_length;
		thiz->find          =  linear_container_darray_find;
		thiz->foreach       =  linear_container_darray_foreach;
		thiz->destroy       =  linear_container_darray_destroy;
	}
	
	return thiz;
}

