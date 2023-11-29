#include "shell.h"

/**
 * get_history_file - Gets the path to the history file.
 *
 * @shell_info: Pointer to the info_t structure.
 *
 * Return: Allocated string containing the history file path.
 */
char *get_history_file(info_t *shell_info)
{
	char *buf, *dir;

	dir = _getenv(shell_info, "HOME=");
	if (!dir)
		return (NULL);
	buf = malloc(sizeof(char) * (_strlen(dir) + _strlen(HIST_FILE) + 2));
	if (!buf)
		return (NULL);
	buf[0] = 0;
	_strcpy(buf, dir);
	_strcat(buf, "/");
	_strcat(buf, HIST_FILE);
	return (buf);
}

/**
 * write_history - Writes the history to a file.
 *
 * @shell_info: Pointer to the info_t structure.
 *
 * Return:
 * - On success: 1
 * - On failure: -1
 */
int write_history(info_t *shell_info)
{
	ssize_t fd;
	char *filename = get_history_file(shell_info);
	list_t *node = NULL;

	if (!filename)
		return (-1);

	fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
	free(filename);
	if (fd == -1)
		return (-1);
	for (node = shell_info->history; node; node = node->next)
	{
		_putsfd(node->str, fd);
		_putfd('\n', fd);
	}
	_putfd(BUF_FLUSH, fd);
	close(fd);
	return (1);
}

/**
 * read_history - Reads history from a file.
 *
 * @shell_info: Pointer to the info_t structure.
 *
 * Return:
 * - On success: Number of history entries.
 * - Otherwise: 0
 */
int read_history(info_t *shell_info)
{
	int i, last = 0, linecount = 0;
	ssize_t fd, rdlen, fsize = 0;
	struct stat st;
	char *buf = NULL, *filename = get_history_file(shell_info);

	if (!filename)
		return (0);

	fd = open(filename, O_RDONLY);
	free(filename);
	if (fd == -1)
		return (0);
	if (!fstat(fd, &st))
		fsize = st.st_size;
	if (fsize < 2)
		return (0);
	buf = malloc(sizeof(char) * (fsize + 1));
	if (!buf)
		return (0);
	rdlen = read(fd, buf, fsize);
	buf[fsize] = 0;
	if (rdlen <= 0)
		return (free(buf), 0);
	close(fd);
	for (i = 0; i < fsize; i++)
		if (buf[i] == '\n')
		{
			buf[i] = 0;
			build_history_list(shell_info, buf + last, linecount++);
			last = i + 1;
		}
	if (last != i)
		build_history_list(shell_info, buf + last, linecount++);
	free(buf);
	info->histcount = linecount;
	while (shell_info->histcount-- >= HIST_MAX)
		delete_node_at_index(&(shell_info->history), 0);
	renumber_history(shell_info);
	return (shell_info->histcount);
}

/**
 * build_history_list - Adds an entry to the history linked list.
 *
 * @shell_info: Pointer to the info_t structure.
 * @buf: The buffer containing the history entry.
 * @linecount: The history line count.
 *
 * Return: Always 0.
 */
int build_history_list(info_t *shell_info, char *buf, int linecount)
{
	list_t *node = NULL;

	if (shell_info->history)
		node = shell_info->history;
	add_node_end(&node, buf, linecount);

	if (!shell_info->history)
		shell_info->history = node;
	return (0);
}

/**
 * renumber_history - Renumbers the history linked list after changes.
 *
 * @shell_info: Pointer to the info_t structure.
 *
 * Return: The new histcount.
 */
int renumber_history(info_t *shell_info)
{
	list_t *node = shell_info->history;
	int i = 0;

	while (node)
	{
		node->num = i++;
		node = node->next;
	}
	return (shell_info->histcount = i);
}
