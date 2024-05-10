#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>
#include "mini_lib.h"
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE_CMD 256
#define MAX_SIZE_ARG 200

char cmd[MAX_SIZE_CMD];
char *argv[MAX_SIZE_ARG];
char tmp[BUF_SIZE];
pid_t pid;
int i;

// démarre le shell

void c_shell()
{

	while (1)
	{
		get_cmd();

		if (!mini_strcmp("exit", cmd))
			break;

		convert_cmd();

		pid = fork();
		if (-1 == pid)
			mini_printf("failed to create a child\n");
		else if (0 == pid)
			execute_command();
		else
			waitpid(pid, NULL, 0);

		int k;
		for (k = 0; k < MAX_SIZE_ARG; k++)
			argv[k] = '\0';
	}
}

void get_cmd()
{

	write(STDOUT_FILENO, "mini_shell> ", 12);

	mini_scanf(cmd, MAX_SIZE_CMD);

	if ((mini_strlen(cmd) > 0) && (cmd[mini_strlen(cmd) - 1] == '\n'))
		cmd[mini_strlen(cmd) - 1] = '\0';
}

void convert_cmd()
{

	char *ptr;
	i = 0;
	ptr = strtok(cmd, " ");
	while (ptr != NULL)
	{

		argv[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}
}

void execute_command()
{
	if (!mini_strcmp(argv[0], "mini_touch"))
	{
		mini_touch(argv[1]);
	}
	else if (!mini_strcmp(argv[0], "mini_cp"))
	{
		mini_cp(argv[1], argv[2]);
	}
	else if (!mini_strcmp(argv[0], "mini_echo"))
	{

		for (int m = 1; argv[m] != NULL; m++)
		{

			mini_echo(argv[m]);
			mini_printf(" ");
		}

		write(STDOUT_FILENO, "\n", 1);
	}
	else if (!mini_strcmp(argv[0], "mini_cat"))
	{
		mini_cat(argv[1]);
	}
	else if (!mini_strcmp(argv[0], "mini_head"))
	{
		mini_head(argv[1], atoi(argv[2]));
	}
	else if (!mini_strcmp(argv[0], "mini_tail"))
	{
		mini_tail(argv[1], atoi(argv[2]));
	}
	else if (!mini_strcmp(argv[0], "mini_clean"))
	{
		mini_clean(argv[1]);
	}
	else if (!mini_strcmp(argv[0], "mini_grep"))
	{
		mini_grep(argv[1], argv[2]);
	}
	else if (!mini_strcmp(argv[0], "mini_wc"))
	{
		mini_wc(argv[1]);
	}
}
