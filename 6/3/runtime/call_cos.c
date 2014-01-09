#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

int main(int argc, char **argv)
{
	void *handle = NULL;
	double (*cosine)(double) = NULL;
	/*加载共享库*/
	handle = dlopen("libm.so", RTLD_LAZY);
	/*通过函数名找到函数指针*/
	*(void **) (&cosine) = dlsym(handle, "cos");
	/*调用函数*/
	printf("%f\n", (*cosine)(2.0));
	/*卸载共享库*/
	dlclose(handle);

	return 0;
}
