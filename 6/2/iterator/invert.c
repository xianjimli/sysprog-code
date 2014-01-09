#include "iterator.h"

Ret invert(Iterator* forward, Iterator* backward)
{
	void* data1 = NULL;
	void* data2 = NULL;
	return_val_if_fail(forward != NULL && backward != NULL, RET_INVALID_PARAMS);

	for(; iterator_offset(forward) < iterator_offset(backward); iterator_next(forward), iterator_prev(backward))
	{
		iterator_get(forward, &data1);
		iterator_get(backward, &data2);
		iterator_set(forward, data2);
		iterator_set(backward, data1);
	}

	return RET_OK;
}

#ifdef INVERT_TEST
#include "dlist.h"
#include "dlist_iterator.h"
#include "test_helper.c"

int main(int argc, char* argv[])
{
	int i = 0;
	int n = 101;
	int last = n - 1;
	DList* dlist = dlist_create(NULL, NULL);

	for(i = 0; i < n; i++)
	{
		dlist_append(dlist, (void*)i);
	}

	Iterator* forward = dlist_iterator_create(dlist);
	Iterator* backward = dlist_iterator_create(dlist);

	iterator_advance(backward, last);
	invert(forward, backward);
	dlist_foreach(dlist, check_and_dec_int, &last);
	iterator_destroy(forward);
	iterator_destroy(backward);
	dlist_destroy(dlist);

	return 0;
}
#endif/*INVERT_TEST*/
