
#include <stdio.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    int n = 0;

	printf("Input number:\n");
	fflush(stdout);
	scanf("%d", &n);
	printf("You input %d\n", n);

    return 0;
}
