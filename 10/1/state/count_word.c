#include <ctype.h>
#include <stdio.h>

#define IS_WORD_CHAR(c)	(isalpha(c) || isdigit(c))

int count_word(const char* text)
{
	enum _State
	{
		STAT_INIT,
		STAT_IN_WORD,
		STAT_OUT_WORD,
	}state = STAT_INIT;

	int count = 0;
	const char* p = text;

	for(p = text; *p != '\0'; p++)
	{
		switch(state)
		{
			case STAT_INIT:
			{
				if(IS_WORD_CHAR(*p))
				{
					state = STAT_IN_WORD;
				}
				else
				{
					state = STAT_OUT_WORD;
				}
				break;
			}
			case STAT_IN_WORD:
			{
				if(!IS_WORD_CHAR(*p))
				{
					count++;
					state = STAT_OUT_WORD;
				}
				break;
			}
			case STAT_OUT_WORD:
			{
				if(IS_WORD_CHAR(*p))
				{
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
	}

	return count;
}

#ifdef COUNT_WORD_TEST
#include <assert.h>

int main(int argc, char* argv[])
{
	assert(count_word("") == 0);
	assert(count_word(" ") == 0);
	assert(count_word("it") == 1);
	assert(count_word("it ") == 1);
	assert(count_word("it is") == 2);
	assert(count_word("it is used to count words.") == 6);
	assert(count_word("it is used to count words. is it easy?") == 9);

	return 0;
}
#endif/*COUNT_WORD_TEST*/

