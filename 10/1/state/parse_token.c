#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef void (*OnTokenFunc)(void* ctx, int index, const char* token);

#define IS_DELIM(c) (strchr(delims, c) != NULL)
int parse_token(const char* text, const char* delims, OnTokenFunc on_token, void* ctx)
{
	enum _State
	{
		STAT_INIT,
		STAT_IN,
		STAT_OUT,
	}state = STAT_INIT;

	int   count     = 0;
	char* copy_text = strdup(text);
	char* p         = copy_text;
	char* token     = copy_text;

	for(p = copy_text; *p != '\0'; p++)
	{
		switch(state)
		{
			case STAT_INIT:
			case STAT_OUT:
			{
				if(!IS_DELIM(*p))
				{
					token = p;
					state = STAT_IN;
				}
				break;
			}
			case STAT_IN:
			{
				if(IS_DELIM(*p))
				{
					*p = '\0';
					on_token(ctx, count++, token);
					state = STAT_OUT;
				}
				break;
			}
			default:break;
		}
	}

	if(state == STAT_IN)
	{
		on_token(ctx, count++, token);
	}

	on_token(ctx, -1, NULL);
	free(copy_text);

	return count;
}

#ifdef PARSE_TOKEN_TEST
#include <assert.h>

void on_token(void* ctx, int index, const char* token)
{
	printf("[%d] %s\n", index, token);

	return;
}

int main(int argc, char* argv[])
{
	assert(parse_token("it is used to token segmentation. is it easy?", " .?", on_token, NULL) == 9);
	assert(parse_token("/backup/tools/jdk1.5.0_18/bin/:/usr/lib/qt-3.3/bin:/usr/kerberos/bin:/backup/tools/jdk1.5.0_18/bin/:/usr/lib/ccache:/usr/local/bin:/usr/bin:/bin:/home/lixianjing/bin", ":", on_token, NULL) == 9);
	assert(parse_token("/backup/tools/jdk1.5.0_18/bin/", "/", on_token, NULL) == 4);

	return 0;
}
#endif/*COUNT_TEST*/

