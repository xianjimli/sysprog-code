#ifndef ALLOCATOR_SELF_MANAGE_H
#define ALLOCATOR_SELF_MANAGE_H
#include "allocator.h"

DECLS_BEGIN

Allocator* allocator_self_manage_create(void* buffer, size_t length);

DECLS_END

#endif/*ALLOCATOR_SELF_MANAGE_H*/

