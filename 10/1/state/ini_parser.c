#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char* strtrim(char* str)
{
	char* p = NULL;

	p = str + strlen(str) - 1;

	while(p != str && isspace(*p)) 
	{
		*p = '\0';
		p--;
	}

	p = str;
	while(*p != '\0' && isspace(*p)) p++;

	if(p != str)
	{
		char* s = p;
		char* d = str;
		while(*s != '\0')
		{
			*d = *s;
			d++;
			s++;
		}
		*d = '\0';
	}

	return str;
}

static void ini_parse_internal(char* buffer, char comment_char, char delim_char)
{
	char* p = buffer;
	char* group_start = NULL;
	char* key_start   = NULL;
	char* value_start = NULL;
	
	enum _State
	{
		STAT_NONE = 0,
		STAT_GROUP,
		STAT_KEY,
		STAT_VALUE,
		STAT_COMMENT
	}state = STAT_NONE;

	for(p = buffer; *p != '\0'; p++)
	{
		switch(state)
		{
			case STAT_NONE:
			{
				if(*p == '[')
				{
					state = STAT_GROUP;
					group_start = p + 1;
				}
				else if(*p == comment_char)
				{
					state = STAT_COMMENT;
				}
				else if(!isspace(*p))
				{
					state = STAT_KEY;
					key_start = p;
				}
				break;
			}
			case STAT_GROUP:
			{
				if(*p == ']')
				{
					*p = '\0';
					state = STAT_NONE;
					strtrim(group_start);
					printf("[%s]\n", group_start);
				}
				break;
			}
			case STAT_COMMENT:
			{
				if(*p == '\n')
				{
					state = STAT_NONE;
					break;
				}
				break;
			}
			case STAT_KEY:
			{
				if(*p == delim_char || (delim_char == ' ' && *p == '\t'))
				{
					*p = '\0';
					state = STAT_VALUE;
					value_start = p + 1;
				}
				break;
			}
			case STAT_VALUE:
			{
				if(*p == '\n' || *p == '\r')
				{
					*p = '\0';
					state = STAT_NONE;
					strtrim(key_start);
					strtrim(value_start);
					printf("%s%c%s\n", key_start, delim_char, value_start);
				}
				break;
			}
			default:break;
		}
	}

	if(state == STAT_VALUE)
	{
		strtrim(key_start);
		strtrim(value_start);
		printf("%s%c%s\n", key_start, delim_char, value_start);
	}

	return;
}

#ifdef INI_PARSER_TEST

int main(int argc, char* argv[])
{
	char* buffer = strdup("[lixianjing]\nname=lixianjing\ngender=male\n[zhangshan]\nname=zhangshan\ngender=male\nage=100");
	ini_parse_internal(buffer, '#', '=');
	free(buffer);

	return 0;
}
#endif/*INI_PARSER_TEST*/

