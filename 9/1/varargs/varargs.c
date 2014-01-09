#include <stdio.h> 

#define va_list void*
#define va_end(arg)
#define va_arg(arg, type)    *(type*)arg; arg = (char*)arg + sizeof(type);
#define va_start(arg, start) arg = (va_list)(((char*)&(start)) + sizeof(start)) 

int accumlate(int nr, ...)
{
    int i = 0;
    int result = 0;
    va_list arg = NULL;
    va_start(arg, nr); 

    for(i = 0; i < nr; i++)
    {
        result += va_arg(arg, int);
    }
    va_end(arg); 

    return result;
}

int main(int argc, char* argv[])
{
    printf("%d\n", accumlate(1, 100));
    printf("%d\n", accumlate(2, 100, 200));
    printf("%d\n", accumlate(3, 100, 200, 300)); 

    return 0;
}

