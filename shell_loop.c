#include "shell.h"

/**
 * hsh - Main shell loop function that drives the interactive shell.
 * @shell_info:  Pointer to the info_t structure.
 * @av: The argument vector.
 *
 * Return:
 * - On successful execution: 0
 * - On error: 1
 * - The exit code of the last executed command.
 */
int hsh(info_t *shell_info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(shell_info);
		if (interactive(shell_info))
			_puts("$ ");
		_eputchar(BUF_FLUSH);
		r = get_input(shell_info);
		if (r != -1)
		{
			set_info(shell_infoinfo, av);
			builtin_ret = find_builtin(shell_info);
			if (builtin_ret == -1)
				find_cmd(shell_info);
		}
		else if (interactive(shell_info))
			_putchar('\n');
		free_info(shell_info, 0);
	}
	write_history(shell_info);
	free_info(shell_info, 1);
	if (!interactive(shell_info) && shell_info->status)
		exit(shell_info->status);
	if (builtin_ret == -2)
	{
		if (shell_info->err_num == -1)
			exit(shell_info->status);
		exit(shell_info->err_num);
	}
	return (builtin_ret);
}

/**
 * find_builtin - Searches for and executes a built-in command.
 * @shell_info: Pointer to the info_t structure.
 *
 * Return:
 * - If no matching built-in command is found: -1
 * - If the built-in command executed successfully: 0
 * - If the built-in command was found but not executed successfully: 1
 * - If the built-in command triggered an exit signal: 2
 */
int find_builtin(info_t *shell_info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(shell_info->argv[0], builtintbl[i].type) == 0)
		{
			shell_info->line_count++;
			built_in_ret = builtintbl[i].func(info);
			break;
		}
	return (built_in_ret);
}

/**
 * find_cmd - Locates and executes a command
 *            from the PATH environment variable
 * @shell_info: Pointer to the info_t structure.
 *
 * Return: void
 */
void find_cmd(info_t *shell_info)
{
	char *path = NULL;
	int i, k;

	shell_info->path = shell_info->argv[0];
	if (shell_info->linecount_flag == 1)
	{
		shell_info->line_count++;
		shell_info->linecount_flag = 0;
	}
	for (i = 0, k = 0; shell_info->arg[i]; i++)
		if (!is_delim(shell_info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(shell_info, _getenv(shell_info, "PATH="),
				shell_info->argv[0]);
	if (path)
	{
		shell_info->path = path;
		fork_cmd(shell_info);
	}
	else
	{
		if ((interactive(shell_info) || _getenv(shell_info, "PATH=")
			|| (shell_info->argv[0][0] == '/') && is_cmd(shell_info,
								shell_info->argv[0]))
			fork_cmd(shell_info);
		else if (*(shell_info->arg) != '\n')
		{
			shell_info->status = 127;
			print_error(shell_info, "not found\n");
		}
	}
}

/**
 * fork_cmd - Forks a child process to execute the specified command.
 * @shell_info:  Pointer to the info_t structure.
 *
 * Return: No return value (void).
 */
void fork_cmd(info_t *shell_info)
{
	pid_t child_pid;

	child_pid = fork();
	if (child_pid == -1)
	{
		/* TODO: PUT ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(shell_info->path, shell_info->argv,
				get_environ(shell_info)) == -1)
		{
			free_info(shell_info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT ERROR FUNCTION */
	}
	else
	{
		wait(&(shell_info->status));
		if (WIFEXITED(shell_info->status))
		{
			shell_info->status = WEXITSTATUS(shell_info->status);
			if (shell_info->status == 126)
				print_error(shell_info, "Permission denied\n");
		}
	}
}
