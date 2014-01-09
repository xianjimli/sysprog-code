#include <stdio.h> 

#define MAX_LEVEL 4
#ifdef NEW_GCC
#define OFFSET 4
#else
#define OFFSET 0
#endif/*NEW_GCC*/ 

int backtrace(void** buffer, int size)
{
	int	 n = 0xfefefefe;
	int* p = &n;
	int	 i = 0; 

	int ebp = p[1 + OFFSET];
	int eip = p[2 + OFFSET]; 

	for(i = 0; i < size; i++)
	{
		buffer[i] = (void*)eip;
		p = (int*)ebp;
		ebp = p[0];
		eip = p[1];
	} 

	return size;
} 

static void test2()
{
	int i = 0;
	void* buffer[MAX_LEVEL] = {0}; 

	backtrace(buffer, MAX_LEVEL); 

	for(i = 0; i < MAX_LEVEL; i++)
	{
		printf("called by %p\n",	buffer[i]);
	} 

	return;
} 

static void test1()
{
	int a=0x11111111;
	int b=0x11111112; 

	test2();
	a = b; 

	return;
} 

static void test()
{
	int a=0x10000000;
	int b=0x10000002; 

	test1();
	a = b; 

	return;
} 

int main(int argc, char* argv[])
{
	test(); 

	return 0;
}

