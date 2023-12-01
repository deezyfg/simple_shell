#include "shell.h"

/**
 * _myhistory - Displays the history list, one command per line,
 *               preceded with line numbers starting at 0.
 *
 * @shell_info: Pointer to structure with potential arguments.
 *
 * Return: Always 0.
 */
int _myhistory(info_t *shell_info)
{
	print_list(shell_info->history);
	return (0);
}

/**
 * unset_alias - Unsets an alias by removing it from the alias list.
 *
 * @shell_info: Pointer to structure with potential arguments.
 * @str: The string alias.
 *
 * Return:
 * - On success: 0
 * - On failure: 1
 */
int unset_alias(info_t *shell_info, char *str)
{
	char *ptr, c;
	int ret;

	ptr = _strchr(str, '=');
	if (!ptr)
		return (1);
	c = *ptr;
	*ptr = 0;
	ret = delete_node_at_index(&(info->alias),
		get_node_index(info->alias, node_starts_with(info->alias, str, -1)));
	*ptr = c;
	return (ret);
}

/**
 * set_alias - Sets an alias to a string.
 *
 * @shell_info: Pointer to structure with potential arguments.
 * @str: The string alias.
 *
 * Return:
 * - On success: 0
 * - On failure: 1
 */
int set_alias(info_t *shell_info, char *str)
{
	char *ptr;

	ptr = _strchr(str, '=');
	if (!ptr)
		return (1);
	if (!*++ptr)
		return (unset_alias(shell_info, str));

	unset_alias(shell_info, str);
	return (add_node_end(&(shell_info->alias), str, 0) == NULL);
}

/**
 * print_alias - Prints an alias string.
 *
 * @node: The alias node.
 *
 * Return:
 * - On success: 0
 * - On failure: 1
 */
int print_alias(list_t *node)
{
	char *ptr = NULL, *a = NULL;

	if (node)
	{
		ptr = _strchr(node->str, '=');
		for (a = node->str; a <= ptr; a++)
			_putchar(*a);
		_putchar('\'');
		_puts(ptr + 1);
		_puts("'\n");
		return (0);
	}
	return (1);
}

/**
 * _myalias - Mimics the alias builtin (man alias).
 *
 * @shell_info: Pointer to structure with potential arguments.
 *
 * Return: Always 0.
 */
int _myalias(info_t *shell_info)
{
	int i = 0;
	char *ptr = NULL;
	list_t *node = NULL;

	if (shell_info->argc == 1)
	{
		node = shell_info->alias;
		while (node)
		{
			print_alias(node);
			node = node->next;
		}
		return (0);
	}
	for (i = 1; shell_info->argv[i]; i++)
	{
		ptr = _strchr(shell_info->argv[i], '=');
		if (ptr)
			set_alias(shell_info, shell_info->argv[i]);
		else
			print_alias(node_starts_with(shell_info->alias, shell_info->argv[i], '='));
	}

	return (0);
}
