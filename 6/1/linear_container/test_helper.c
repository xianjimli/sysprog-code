static int hash_int(void* data)
{
	return (int)data;
}

static int cmp_int(void* ctx, void* data)
{
	return (int)ctx - (int)data;
}

static Ret print_int(void* ctx, void* data)
{
	printf("%d ", (int)data);

	return RET_OK;
}

static Ret check_and_dec_int(void* ctx, void* data)
{
	int* expected =(int*)ctx;
	assert(*expected == (int)data);

	(*expected)--;

	return RET_OK;
}

