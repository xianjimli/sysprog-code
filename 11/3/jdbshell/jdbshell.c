/*
 * File:    jdbshell.c 
 * Author:  Li XianJing <lixianjing@broncho.cn>
 * Brief:   a shell for gdb.
 *
 * Copyright (c) 2009  topwise, Inc.
 *
 * Licensed under the Academic Free License version 2.1
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/*
 * History:
 * ================================================================
 * 2009-07-08 Li XianJing <lixianjing@broncho.cn> created
 *
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <signal.h>

static const char* commands[] = 
{
	"class",
	"classes",
	"classpath",
	"clear",
	"connectors",
	"cont",
	"disablegc",
	"down",
	"dump",
	"enablegc",
	"eval",
	"exit",
	"fields",
	"help",
	"interrupt",
	"kill",
	"list",
	"locals",
	"lock",
	"methods",
	"monitor",
	"next",
	"pop",
	"print",
	"read",
	"reenter",
	"resume",
	"run",
	"set",
	"step",
	"stepi",
	"stop",
	"suspend",
	"thread",
	"threadgroup",
	"threadgroups",
	"threadlocks",
	"threads",
	"trace",
	"unmonitor",
	"untrace",
	"up",
	"version",
	"where",
	"wherei",
	NULL
};

struct Alias
{
	const char* alias;
	const char* command;
}g_alias[] =
{
	{"c",  "cont"},
	{"l",  "list"},
	{"n",  "next"},
	{"r",  "run"},
	{"s",  "step"},
	{"si", "stepi"},
	{"f",  "step up"},
	{"bt", "wherei"},
	{NULL, NULL}
};

char* command_generator(const char *text, int state)
{
	const char *name;
	static int list_index, len;

	if (!state)
	{
		list_index = 0;
		len = strlen (text);
	}
	
	while (name = commands[list_index])
	{
		list_index++;
		
		if (strncmp (name, text, len) == 0)
			return strdup(name);
	}

	return ((char *)NULL);
}

char** command_completion (const char *text, int start, int end)
{
	char **matches = NULL;

	if (start == 0)
		matches = rl_completion_matches (text, command_generator);

	return (matches);
}

void initialize_readline ()
{
	rl_readline_name = "jdbshell";
	rl_attempted_completion_function = command_completion;

	return ;
}

char * stripwhite (char *string)
{
	char *s = NULL;
	char *t = NULL;
	
	for (s = string; whitespace (*s); s++);
    
	if (*s == 0) return (s);

	t = s + strlen (s) - 1;
	while (t > s && whitespace (*t))t--;
	*++t = '\0';

	return s;
}

void show_usage(void)
{
	int i = 0;
	fprintf(stderr, "usage: jdbshell [jdb args]\n");
	fprintf(stderr, "example: ./jdbshell -sourcepath $ANDROID_SRC_PATH -attach 6107\n");
	fprintf(stderr, "command alias:\n");

	for(i = 0; g_alias[i].alias != NULL; i++)
	{
		fprintf(stderr, "	%2s -- %s\n", g_alias[i].alias, g_alias[i].command);
	}

	return;
}

int main (int argc, char **argv)
{
	int  pid = 0;
	char *line = NULL;
	char *s = NULL;
	char last_cmd[1024] = {0};
	int  parent_to_child[2] = {0};
	
	if(argc == 1)
	{
		show_usage();
		return 0;
	}

	initialize_readline ();
	pipe(parent_to_child);

	pid = fork();
	if(pid == 0)
	{
		close(parent_to_child[1]);
		dup2(parent_to_child[0], STDIN_FILENO);
		strcpy(argv[0], "jdb");
		int ret = execvp("jdb", argv);
		fprintf(stderr, "jdb exit: ret=%d\n", ret);
	}
	else
	{
		close(parent_to_child[0]);

		while(1)
		{
			int i = 0;
			line = readline ("");
			
			if (!line) break;

			s = stripwhite (line);
			
			if(*s)
			{
			  add_history (s);
			  strncpy(last_cmd, s, sizeof(last_cmd)-1);
			}
			else if(last_cmd[0])
			{
				s = last_cmd;
				printf(">%s\n", s);
			}
			else
			{
				free (line);
				printf(">");
				continue;
			}

			for(i = 0; g_alias[i].alias != NULL; i++)
			{
				int len = strlen(g_alias[i].alias);
				if(strncmp(g_alias[i].alias, s, len) == 0 
					&& (s[len] == ' ' || s[len] == '\0' || s[len] == '\t'))
				{
					write(parent_to_child[1], g_alias[i].command, 
						strlen(g_alias[i].command));
					break;
				}
			}
		
			if(g_alias[i].alias == NULL)
			{
				write(parent_to_child[1], s, strlen(s));
			}

			if(write(parent_to_child[1], "\n", 1) <= 0)
			{
				fprintf(stderr, "jdb exited\n");
				break;
			}
			free (line);
		}

		kill(pid, SIGTERM);
	}

	return 0;
}
