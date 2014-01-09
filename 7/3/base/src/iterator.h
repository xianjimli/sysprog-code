#ifndef ITERATOR_H
#define ITERATOR_H

#include "typedef.h"

DECLS_BEGIN

struct _Iterator;
typedef struct _Iterator Iterator;

typedef Ret  (*IteratorSetFunc)(Iterator* thiz, void* data);
typedef Ret  (*IteratorGetFunc)(Iterator* thiz, void** data);
typedef Ret  (*IteratorNextFunc)(Iterator* thiz);
typedef Ret  (*IteratorPrevFunc)(Iterator* thiz);
typedef Ret  (*IteratorAdvanceFunc)(Iterator* thiz, int offset);
typedef int  (*IteratorOffsetFunc)(Iterator* thiz);
typedef Ret  (*IteratorCloneFunc)(Iterator* thiz, Iterator** cloned);
typedef void (*IteratorDestroyFunc)(Iterator* thiz);

struct _Iterator
{
	IteratorSetFunc     set;
	IteratorGetFunc     get;
	IteratorNextFunc    next;
	IteratorPrevFunc    prev;
	IteratorAdvanceFunc advance;
	IteratorCloneFunc   clone;
	IteratorOffsetFunc  offset;
	IteratorDestroyFunc destroy;

	char priv[0];
};

static inline Ret  iterator_set(Iterator* thiz, void* data)
{
	return_val_if_fail(thiz != NULL && thiz->set != NULL, RET_INVALID_PARAMS);

	return thiz->set(thiz, data);
}

static inline Ret  iterator_get(Iterator* thiz, void** data)
{
	return_val_if_fail(thiz != NULL && thiz->get != NULL, RET_INVALID_PARAMS);

	return thiz->get(thiz, data);
}

static inline Ret  iterator_next(Iterator* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->next != NULL, RET_INVALID_PARAMS);

	return thiz->next(thiz);
}

static inline Ret  iterator_prev(Iterator* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->prev != NULL, RET_INVALID_PARAMS);

	return thiz->prev(thiz);
}

static inline Ret  iterator_advance(Iterator* thiz, int offset)
{
	return_val_if_fail(thiz != NULL && thiz->advance != NULL, RET_INVALID_PARAMS);

	return thiz->advance(thiz, offset);
}

static inline int  iterator_offset(Iterator* thiz)
{
	return_val_if_fail(thiz != NULL && thiz->offset != NULL, -1);

	return thiz->offset(thiz);
}

static inline Ret  iterator_clone(Iterator* thiz, Iterator** cloned)
{
	return_val_if_fail(thiz != NULL && thiz->clone != NULL, RET_INVALID_PARAMS);

	return thiz->clone(thiz, cloned);
}

static inline void iterator_destroy(Iterator* thiz)
{
	return_if_fail(thiz != NULL && thiz->destroy != NULL);

	return thiz->destroy(thiz);
}

DECLS_END

#endif/*ITERATOR_H*/

