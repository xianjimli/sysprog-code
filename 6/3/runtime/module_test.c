#include <stdio.h>
#include "module.h"

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("%s module function\n", argv[0]);

		return 0;
	}

	Module* module = module_create(argv[1], 0);
	if(module != NULL)
	{
		void* func = module_sym(module, argv[2]);
		printf("func=%p\n", func);
		module_destroy(module);
	}
	else
	{
		printf("load %s failed\n", argv[1]);
	}

	return 0;
}
