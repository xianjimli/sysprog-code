#include "module.h"
#include "queue.h"
#include "test_helper.c"
#include "linear_container.h"

typedef LinearContainer* (*LinearContainerCreateFunc)(DataDestroyFunc data_destroy, void* ctx);

int main(int argc, char* argv[])
{
	int i = 0;
	int n = 1000;
	int ret_data = 0;
	Queue* queue = NULL;
	Module* module = NULL;
	LinearContainerCreateFunc linear_container_create = NULL;
	if(argc != 3)
	{
		printf("%s sharelib linear_container_create\n", argv[0]);

		return 0;
	}

	module = module_create(argv[1], 0);
	return_val_if_fail(module != NULL, 0);
	linear_container_create = (LinearContainerCreateFunc)module_sym(module, argv[2]);
	if(linear_container_create == NULL)
	{
		module_destroy(module);

		return 0;
	}

	queue = queue_create(linear_container_create(NULL, NULL));

	for(i = 0; i < n; i++)
	{
		assert(queue_push(queue, (void*)i) == RET_OK);
		assert(queue_head(queue, (void**)&ret_data) == RET_OK);
		assert(queue_length(queue) == (i+1));
	}

	queue_foreach(queue, print_int, NULL);

	for(i = 0; i < n; i++)
	{
		assert(queue_head(queue, (void**)&ret_data) == RET_OK);
		assert(ret_data == i );
		assert(queue_length(queue) == (n - i));
		assert(queue_pop(queue) == RET_OK);
	}

	queue_destroy(queue);
	module_destroy(module);

	return 0;
}
