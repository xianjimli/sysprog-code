#include <stdio.h>
#include <pthread.h>

static pthread_key_t key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static void make_key(void)
{
	pthread_key_create(&key, NULL);

	return;
}

void* thread_entry(void* param)
{
	pthread_once(&key_once, make_key);

	if (pthread_getspecific(key) == NULL)
	{
        pthread_setspecific(key, (void*)pthread_self());
    }

	printf("data=%u\n", pthread_getspecific(key));

	return NULL;
}

#define THREAD_NR 5
int main(int argc, char* argv[])
{
	int i = 0;
	pthread_t tids[THREAD_NR] = {0};

	for(i = 0; i < THREAD_NR; i++)
	{
		pthread_create(tids+i, NULL, thread_entry, NULL);

	}

	for(i = 0; i < THREAD_NR; i++)
	{
		pthread_join(tids[i], NULL);
	}

	return 0;
}
