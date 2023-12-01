#include "shell.h"

/**
 * input_buf - Buffers chained commands.
 *
 * @shell_info: Pointer to the info_t structure.
 * @buf: Address of the buffer.
 * @len: Address of the length variable.
 *
 * Return: Bytes read.
 */
ssize_t input_buf(info_t *shell_info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len)  /* If nothing left in the buffer, fill it. */
	{
		/*bfree((void **)shell_info->cmd_buf);*/
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE
		r = getline(buf, &len_p, stdin);
#else
		r = _getline(shell_info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0'; /* Remove trailing newline. */
				r--;
			}
			shell_info->linecount_flag = 1;
			remove_comments(*buf);
			build_history_list(shell_info, *buf, shell_info->histcount++);
			/* if (_strchr(*buf, ';')) /* Check if it's a command chain. */
			{
				*len = r;
				shell_info->cmd_buf = buf;
			}
		}
	}
	return (r);
}

/**
 * get_input - Gets a line minus the newline.
 *
 * @shell_info: Pointer to the info_t structure.
 *
 * Return: Bytes read.
 */

ssize_t get_input(info_t *shell_info)
{
	static char *buf; /* the ';' command chain buffer */
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(shell_info->arg), *p;

	_putchar(BUF_FLUSH);
	r = input_buf(shell_info, &buf, &len);
	if (r == -1) /* EOF */
		return (-1);
	if (len)	/* We have commands left in the chain buffer. */
	{
		j = i; /* Initialize new iterator to the current buffer position. */
		p = buf + i; /* Get pointer for return. */

		check_chain(shell_info, buf, &j, i, len);
		while (j < len) /* Iterate to semicolon or end. */
		{
			if (is_chain(shell_info, buf, &j))
				break;
			j++;
		}

		i = j + 1; /* Increment past nulled ';'. */
		if (i >= len) /* Reached the end of the buffer? */
		{
			i = len = 0; /* Reset position and length. */
			shell_info->cmd_buf_type = CMD_NORM;
		}

		*buf_p = p; /* Pass back pointer to the current command position. */
		return (_strlen(p)); /* Return length of the current command. */
	}

	*buf_p = buf; /* Else not a chain, pass back buffer from _getline(). */
	return (r); /* Return length of buffer from _getline(). */
}

/**
 * read_buf - Reads a buffer.
 *
 * @shell_info: Pointer to the info_t structure.
 * @buf: Buffer.
 * @i: Size.
 *
 * Return: r.
 */

ssize_t read_buf(info_t *shell_info, char *buf, size_t *i)
{
	ssize_t r = 0;

	if (*i)
		return (0);
	r = read(shell_info->readfd, buf, READ_BUF_SIZE);
	if (r >= 0)
		*i = r;
	return (r);
}

/**
 * _getline - Gets the next line of input from STDIN.
 *
 * @shell_info: Pointer to the info_t structure.
 * @ptr: Address of the pointer to the buffer, preallocated or NULL.
 * @length: Size of the preallocated ptr buffer if not NULL.
 *
 * Return: s.
 */

int _getline(info_t *shell_info, char **ptr, size_t *length)
{
	static char buf[READ_BUF_SIZE];
	static size_t i, len;
	size_t k;
	ssize_t r = 0, s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (i == len)
		i = len = 0;

	r = read_buf(shell_info, buf, &len);
	if (r == -1 || (r == 0 && len == 0))
		return (-1);

	c = _strchr(buf + i, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p) /* MALLOC FAILURE! */
		return (p ? free(p), -1 : -1);

	if (s)
		_strncat(new_p, buf + i, k - i);
	else
		_strncpy(new_p, buf + i, k - i + 1);

	s += k - i;
	i = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}

/**
 * sigintHandler - Blocks ctrl-C.
 *
 * @sig_num: The signal number.
 *
 * Return: void.
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(BUF_FLUSH);
}

