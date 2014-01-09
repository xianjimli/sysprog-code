#include "dlist.h"
#include "linear_container.h"

typedef struct _PrivInfo
{
	DList* dlist;
}PrivInfo;

static Ret linear_container_dlist_insert(LinearContainer* thiz, size_t index, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_insert(priv->dlist, index, data);
}

static Ret linear_container_dlist_prepend(LinearContainer* thiz, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_prepend(priv->dlist, data);
}

static Ret linear_container_dlist_append(LinearContainer* thiz, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_append(priv->dlist, data);
}

static Ret linear_container_dlist_delete(LinearContainer* thiz, size_t index)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_delete(priv->dlist, index);
}

static Ret linear_container_dlist_get_by_index(LinearContainer* thiz, size_t index, void** data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_get_by_index(priv->dlist, index, data);
}

static Ret linear_container_dlist_set_by_index(LinearContainer* thiz, size_t index, void* data)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_set_by_index(priv->dlist, index, data);
}

static size_t linear_container_dlist_length(LinearContainer* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_length(priv->dlist);
}

static int    linear_container_dlist_find(LinearContainer* thiz, DataCompareFunc cmp, void* ctx)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_find(priv->dlist, cmp, ctx);
}

static Ret    linear_container_dlist_foreach(LinearContainer* thiz, DataVisitFunc visit, void* ctx)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	return dlist_foreach(priv->dlist, visit, ctx);
}

static void   linear_container_dlist_destroy(LinearContainer* thiz)
{
	PrivInfo* priv = (PrivInfo*)thiz->priv;

	dlist_destroy(priv->dlist);
	free(thiz);

	return;
}

LinearContainer* linear_container_dlist_create(DataDestroyFunc data_destroy, void* ctx)
{
	LinearContainer* thiz = (LinearContainer*)malloc(sizeof(LinearContainer) + sizeof(PrivInfo));

	if(thiz != NULL)
	{
		PrivInfo* priv = (PrivInfo*)thiz->priv;
		priv->dlist = dlist_create(data_destroy, ctx);

		thiz->insert        =  linear_container_dlist_insert;
		thiz->prepend       =  linear_container_dlist_prepend;
		thiz->append        =  linear_container_dlist_append;
		thiz->del           =  linear_container_dlist_delete;
		thiz->get_by_index  =  linear_container_dlist_get_by_index;
		thiz->set_by_index  =  linear_container_dlist_set_by_index;
		thiz->length        =  linear_container_dlist_length;
		thiz->find          =  linear_container_dlist_find;
		thiz->foreach       =  linear_container_dlist_foreach;
		thiz->destroy       =  linear_container_dlist_destroy;
	}
	
	return thiz;
}

