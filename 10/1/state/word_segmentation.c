#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define IS_WORD_CHAR(c)	(isalpha(c) || isdigit(c))

typedef void (*OnWordFunc)(void* ctx, const char* word);

int word_segmentation(const char* text, OnWordFunc on_word, void* ctx)
{
	enum _State
	{
		STAT_INIT,
		STAT_IN_WORD,
		STAT_OUT_WORD,
	}state = STAT_INIT;

	int count = 0;
	char* copy_text = strdup(text);
	char* p = copy_text;
	char* word = copy_text;

	for(p = copy_text; *p != '\0'; p++)
	{
		switch(state)
		{
			case STAT_INIT:
			{
				if(IS_WORD_CHAR(*p))
				{
					word = p;
					state = STAT_IN_WORD;
				}
				break;
			}
			case STAT_IN_WORD:
			{
				if(!IS_WORD_CHAR(*p))
				{
					count++;
					*p = '\0';
					on_word(ctx, word);
					state = STAT_OUT_WORD;
				}
				break;
			}
			case STAT_OUT_WORD:
			{
				if(IS_WORD_CHAR(*p))
				{
					word = p;
					state = STAT_IN_WORD;
				}
				break;
			}
			default:break;
		}
	}

	if(state == STAT_IN_WORD)
	{
		count++;
		on_word(ctx, word);
	}

	free(copy_text);

	return count;
}

#ifdef WORD_SEGMENTATION_TEST
#include <assert.h>

void on_word(void* ctx, const char* word)
{
	printf("%s\n", word);

	return;
}

int main(int argc, char* argv[])
{
	assert(word_segmentation("it is used to word segmentation. is it easy?", on_word, NULL) == 9);

	return 0;
}
#endif/*WORD_SEGMENTATION_TEST*/

