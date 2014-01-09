#ifndef LINEAR_CONTAINER_H
#define LINEAR_CONTAINER_H

#include "typedef.h"

struct _LinearContainer;
typedef struct _LinearContainer LinearContainer;

typedef Ret (*LinearContainerInsertFunc)(LinearContainer* thiz, size_t index, void* data);
typedef Ret (*LinearContainerPrependFunc)(LinearContainer* thiz, void* data);
typedef Ret (*LinearContainerAppendFunc)(LinearContainer* thiz, void* data);
typedef Ret (*LinearContainerDeleteFunc)(LinearContainer* thiz, size_t index);
typedef Ret (*LinearContainerGetByIndexFunc)(LinearContainer* thiz, size_t index, void** data);
typedef Ret (*LinearContainerSetByIndexFunc)(LinearContainer* thiz, size_t index, void* data);
typedef size_t (*LinearContainerLengthFunc)(LinearContainer* thiz);
typedef int    (*LinearContainerFindFunc)(LinearContainer* thiz, DataCompareFunc cmp, void* ctx);
typedef Ret    (*LinearContainerForeachFunc)(LinearContainer* thiz, DataVisitFunc visit, void* ctx);
typedef void   (*LinearContainerDestroyFunc)(LinearContainer* thiz);

struct _LinearContainer
{
	LinearContainerInsertFunc     insert;
	LinearContainerPrependFunc    prepend;
	LinearContainerAppendFunc     append;
	LinearContainerDeleteFunc     del;
	LinearContainerGetByIndexFunc get_by_index;
	LinearContainerSetByIndexFunc set_by_index;
	LinearContainerLengthFunc     length;
	LinearContainerFindFunc       find;
	LinearContainerForeachFunc    foreach;
	LinearContainerDestroyFunc    destroy;

	char priv[0];
};

static inline Ret linear_container_insert(LinearContainer* thiz, size_t index, void* data)
{
	return_val_if_fail(thiz != NULL && thiz->insert != NULL, RET_INVALID_PARAMS);

	return thiz->insert(thiz, index, data);
}

static inline Ret linear_container_prepend(LinearContainer* thiz, void* data)
{
	return_val_if_fail(thiz != NULL && thiz->prepend != NULL, RET_INVALID_PARAMS);

	return thiz->prepend(thiz, data);
}

static inline Ret linear_container_append(LinearContainer* thiz, void* data)
{
	return_val_if_fail(thiz != NULL && thiz->append != NULL, RET_INVALID_PARAMS);

	return thiz->append(thiz, data);
}

static inline Ret linear_container_delete(LinearContainer* thiz, size_t index)
{
	return_val_if_fail(thiz != NULL && thiz->del != NULL, RET_INVALID_PARAMS);

	return thiz->del(thiz, index);
}

static inline Ret linear_container_get_by_index(LinearContainer* thiz, size_t index, void** data)
{
	return_val_if_fail(thiz != NULL && thiz->get_by_index != NULL, RET_INVALID_PARAMS);

	return thiz->get_by_index(thiz, index, data);
}

static inline Ret linear_container_set_by_index(LinearContainer* thiz, size_t index, void* data)
{
	return_val_if_fail(thiz != NULL && thiz->set_by_index != NULL, RET_INVALID_PARAMS);

	return thiz->set_by_index(thiz, index, data);
}

static inline size_t linear_container_length(LinearContainer* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->length != NULL, 0);

	return thiz->length(thiz);
}

static inline int    linear_container_find(LinearContainer* thiz, DataCompareFunc cmp, void* ctx)
{
	return_val_if_fail(thiz != NULL && thiz->find != NULL, -1);

	return thiz->find(thiz, cmp, ctx);
}

static inline Ret    linear_container_foreach(LinearContainer* thiz, DataVisitFunc visit, void* ctx)
{
	return_val_if_fail(thiz != NULL && thiz->foreach != NULL, RET_INVALID_PARAMS);

	return thiz->foreach(thiz, visit, ctx);
}

static inline void   linear_container_destroy(LinearContainer* thiz)
{
	return_if_fail(thiz!= NULL && thiz->destroy != NULL);

	return thiz->destroy(thiz);
}

#endif/*LINEAR_CONTAINER_H*/

