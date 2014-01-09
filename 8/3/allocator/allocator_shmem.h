#ifndef ALLOCATOR_SHMEM_H
#define ALLOCATOR_SHMEM_H
#include "allocator.h"

DECLS_BEGIN

Allocator* allocator_shmem_create(void* addr, size_t init_size);

DECLS_END

#endif/*ALLOCATOR_SHMEM_H*/

