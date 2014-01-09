#include <dlist.h>

int main(int argc, char* argv[])
{
	DList* dlist = dlist_create(NULL, NULL);

	dlist_destroy(dlist);

	return 0;
}
