#include <stdio.h>
#include "dlist.h"
#include <assert.h>

static DListRet sum_cb(void* ctx, void* data)
{
	long long* result = ctx;
	*result += (int)data;

	return DLIST_RET_OK;
}

typedef struct _MaxCtx
{
	int is_first;
	int max;
}MaxCtx;

static DListRet max_cb(void* ctx, void* data)
{
	MaxCtx* max_ctx = ctx;
	if(max_ctx->is_first)
	{
		max_ctx->is_first = 0;
		max_ctx->max = (int)data;
	}
	else if(max_ctx->max < (int)data)
	{
		max_ctx->max = (int)data;
	}

	return DLIST_RET_OK;
}

static DListRet print_int(void* ctx, void* data)
{
	printf("%d ", (int)data);

	return DLIST_RET_OK;
}

int main(int argc, char* argv[])
{
	int i = 0;
	int n = 100;
	long long sum = 0;
	MaxCtx max_ctx = {.is_first = 1, 0};
	DList* dlist = dlist_create();

	for(i = 0; i < n; i++)
	{
		assert(dlist_append(dlist, (void*)i) == DLIST_RET_OK);
	}

	dlist_foreach(dlist, print_int, NULL);
	dlist_foreach(dlist, max_cb, &max_ctx);
	dlist_foreach(dlist, sum_cb, &sum);

	printf("\nsum=%lld max=%d\n", sum, max_ctx.max);
	dlist_destroy(dlist);

	return 0;
}

