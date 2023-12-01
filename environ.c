#include "shell.h"

/**
 * _myenv - Prints the current environment.
 * @shell_info: Pointer to structure with potential arguments.
 *
 * Return: Always 0
 */
int _myenv(info_t *shell_info)
{
	print_list_str(shell_info->env);
	return (0);
}

/**
 * _getenv - Gets the value of an environment variable.
 * @shell_info: Pointer to structure with potential arguments.
 * @name: Name of the environment variable.
 *
 * Return: The value of the environment variable, or NULL if not found.
 */

char *_getenv(info_t *shell_info, const char *name)
{
	list_t *node = shell_info->env;
	char *ptr;

	while (node)
	{
		ptr = starts_with(node->str, name);
		if (ptr && *ptr)
			return (ptr);
		node = node->next;
	}
	return (NULL);
}

/**
 * _mysetenv - Initializes a new environment variable
 *             or modifies an existing one.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *  Return: Always 0
 */

int _mysetenv(info_t *shell_info)
{
	if (shell_info->argc != 3)
	{
		_eputs("Incorrect number of arguements\n");
		return (1);
	}
	if (_setenv(info, shell_info->argv[1], info->argv[2]))
		return (0);
	return (1);
}

/**
 * _myunsetenv - Removes an environment variable.
 * @shell_info: Pointer to structure with potential arguments.
 *
 *  Return: Always 0.
 */

int _myunsetenv(info_t *shell_info)
{
	int i;

	if (shell_info->argc == 1)
	{
		_eputs("Too few arguements.\n");
		return (1);
	}
	for (i = 1; i <= shell_info->argc; i++)
		_unsetenv(shell_info, shell_info->argv[i]);

	return (0);
}

/**
 * populate_env_list - populates env linked list
 * @shell_info: Pointer to structure with potential arguments.
 *
 * Return: Always 0
 */

int populate_env_list(info_t *info)
{
	list_t *node = NULL;
	size_t i;

	for (i = 0; environ[i]; i++)
		add_node_end(&node, environ[i], 0);
	shell_info->env = node;
	return (0);
}
