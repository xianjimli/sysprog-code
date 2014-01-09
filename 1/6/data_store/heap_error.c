#include <stdlib.h>
#include <string.h>
/*# valgrind --tool=memcheck --leak-check=full ./heap_error */
int main(int argc, char* argv[])
{
	/*memory leak and buffer overflow.*/
	char* str = malloc(10);
	strcpy(str, "123456788900");

	return 0;
}
