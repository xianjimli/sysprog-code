#ifndef ALLOCATOR_CHECKBO_H
#define ALLOCATOR_CHECKBO_H
#include "allocator.h"

DECLS_BEGIN

Allocator* allocator_checkbo_create(Allocator* real_allocator);

DECLS_END

#endif/*ALLOCATOR_CHECKBO_H*/

