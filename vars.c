#include "shell.h"

/**
 * is_chain - Tests if the current character
 *            in the buffer is a chain delimiter.
 *
 * @shell_info: Pointer to the info_t structure.
 * @buf: The character buffer.
 * @ptr: Address of the current position in buf.
 *
 * Return:
 * - If it's a chain delimiter: 1
 * - Otherwise: 0
 */
int is_chain(info_t *shell_info, char *buf, size_t *ptr)
{
	size_t j = *ptr;

	if (buf[j] == '|' && buf[j + 1] == '|')
	{
		buf[j] = 0;
		j++;
		shell_info->cmd_buf_type = CMD_OR;
	}
	else if (buf[j] == '&' && buf[j + 1] == '&')
	{
		buf[j] = 0;
		j++;
		shell_info->cmd_buf_type = CMD_AND;
	}
	else if (buf[j] == ';') /* Found end of this command */
	{
		buf[j] = 0; /* Replace semicolon with null */
		shell_info->cmd_buf_type = CMD_CHAIN;
	}
	else
		return (0);
	*ptr = j;
	return (1);
}

/**
 * check_chain - Checks if we should continue
 *               chaining based on the last status.
 *
 * @shell_info: Pointer to the info_t structure.
 * @buf: The character buffer.
 * @ptr: Address of the current position in buf.
 * @i: Starting position in buf.
 * @len: Length of buf.
 *
 * Return: Void.
 */
void check_chain(info_t *shell_info, char *buf, size_t *ptr,
			size_t i, size_t len)
{
	size_t j = *ptr;

	if (shell_info->cmd_buf_type == CMD_AND)
	{
		if (shell_info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}
	if (shell_info->cmd_buf_type == CMD_OR)
	{
		if (!shell_info->status)
		{
			buf[i] = 0;
			j = len;
		}
	}

	*ptr = j;
}

/**
 * replace_alias - Replaces aliases in the tokenized string.
 *
 * @shell_info: Pointer to the info_t structure.
 *
 * Return:
 * - If replaced: 1
 * - otherwise: 0
 */
int replace_alias(info_t *shell_info)
{
	int i;
	list_t *node;
	char *p;

	for (i = 0; i < 10; i++)
	{
		node = node_starts_with(shell_info->alias, shell_info->argv[0], '=');
		if (!node)
			return (0);
		free(shell_info->argv[0]);
		p = _strchr(node->str, '=');
		if (!p)
			return (0);
		p = _strdup(p + 1);
		if (!p)
			return (0);
		shell_info->argv[0] = p;
	}
	return (1);
}

/**
 * replace_vars - Replaces variables in the tokenized string.
 *
 * @shell_info: Pointer to the info_t structure.
 *
 * Return:
 * - If replaced: 1
 * - otherwise: 0
 */
int replace_vars(info_t *shell_info)
{
	int i = 0;
	list_t *node;

	for (i = 0; shell_info->argv[i]; i++)
	{
		if (shell_info->argv[i][0] != '$' || !shell_info->argv[i][1])
			continue;

		if (!_strcmp(shell_info->argv[i], "$?"))
		{
			replace_string(&(shell_info->argv[i]),
				_strdup(convert_number(shell_info->status, 10, 0)));
			continue;
		}
		if (!_strcmp(shell_info->argv[i], "$$"))
		{
			replace_string(&(shell_info->argv[i]),
				_strdup(convert_number(getpid(), 10, 0)));
			continue;
		}
		node = node_starts_with(shell_info->env, &shell_info->argv[i][1], '=');
		if (node)
		{
			replace_string(&(shell_info->argv[i]),
				_strdup(_strchr(node->str, '=') + 1));
			continue;
		}
		replace_string(&shell_info->argv[i], _strdup(""));

	}
	return (0);
}

/**
 * replace_string - Replaces a string.
 *
 * @old: Address of the old string.
 * @new: New string.
 *
 * Return:
 * - If replaced: 1
 * - otherwise: 0
 */
int replace_string(char **old, char *new)
{
	free(*old);
	*old = new;
	return (1);
}
