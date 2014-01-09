#ifndef ALLOCATOR_POOL_H
#define ALLOCATOR_POOL_H
#include "allocator.h"

DECLS_BEGIN

Allocator* allocator_pool_create(size_t expand_delta);

DECLS_END

#endif/*ALLOCATOR_POOL_H*/

