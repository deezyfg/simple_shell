#include "shell.h"

/**
 * get_environ - Returns the string array copy of our environ.
 *
 * @shell_info: Pointer to the info_t structure.
 *
 * Return: Pointer to the array of environment strings.
 */
char **get_environ(info_t *shell_info)
{
	if (!shell_info->environ || shell_info->env_changed)
	{
		shell_info->environ = list_to_strings(shell_info->env);
		shell_info->env_changed = 0;
	}

	return (shell_info->environ);
}

/**
 * _unsetenv - Removes an environment variable.
 *
 * @shell_info: Pointer to the info_t structure.
 * @var: The string representing the environment variable.
 *
 * Return: 1 on success (delete), 0 otherwise.
 */

int _unsetenv(info_t *shell_info, char *var)
{
	list_t *node = shell_info->env;
	size_t i = 0;
	char *ptr;

	if (!node || !var)
		return (0);

	while (node)
	{
		ptr = starts_with(node->str, var);
		if (ptr && *ptr == '=')
		{
			shell_info->env_changed = delete_node_at_index(&(shell_info->env), i);
			i = 0;
			node = shell_info->env;
			continue;
		}
		node = node->next;
		i++;
	}
	return (shell_info->env_changed);
}

/**
 * _setenv - Initializes a new environment variable
 *           or modifies an existing one.
 *
 * @shell_info: Pointer to the info_t structure.
 * @var: The string representing the environment variable.
 * @value: The string representing the environment variable's value.
 *
 * Return: Always 0.
 */

int _setenv(info_t *shell_info, char *var, char *value)
{
	char *buf = NULL;
	list_t *node;
	char *ptr;

	if (!var || !value)
		return (0);

	buf = malloc(_strlen(var) + _strlen(value) + 2);
	if (!buf)
		return (1);
	_strcpy(buf, var);
	_strcat(buf, "=");
	_strcat(buf, value);
	node = shell_info->env;
	while (node)
	{
		ptr = starts_with(node->str, var);
		if (ptr && *ptr == '=')
		{
			free(node->str);
			node->str = buf;
			shell_info->env_changed = 1;
			return (0);
		}
		node = node->next;
	}
	add_node_end(&(shell_info->env), buf, 0);
	free(buf);
	shell_info->env_changed = 1;
	return (0);
}
