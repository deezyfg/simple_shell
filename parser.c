#include "shell.h"

/**
 * is_cmd - Determines if a file is an executable command.
 *
 * @shell_info: Pointer to the info_t structure.
 * @path: Path to the file.
 *
 * Return:
 * - If the file is an executable command: 1
 * - Otherwise: 0
 */
int is_cmd(info_t *shell_info, char *path)
{
	struct stat st;

	(void)shell_info;
	if (!path || stat(path, &st))
		return (0);

	if (st.st_mode & S_IFREG)
	{
		return (1);
	}
	return (0);
}

/**
 * dup_chars - Duplicates characters from a string.
 *
 * @pathstr: The PATH string.
 * @start: Starting index.
 * @stop: Stopping index.
 *
 * Return: Pointer to the new buffer.
 */

char *dup_chars(char *pathstr, int start, int stop)
{
	static char buf[1024];
	int i = 0, j = 0;

	for (j = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[j++] = pathstr[i];
	buf[j] = 0;
	return (buf);
}


/**
 * find_path - Finds the command in the PATH string.
 *
 * @shell_info: Pointer to the info_t structure.
 * @pathstr: The PATH string.
 * @cmd: The command to find.
 *
 * Return:
 * - Full path of the command if found.
 * - NULL otherwise.
 */

char *find_path(info_t *shell_info, char *pathstr, char *cmd)
{
	int i = 0, current_position = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if ((_strlen(cmd) > 2) && starts_with(cmd, "./"))
	{
		if (is_cmd(shell_info, cmd))
			return (cmd);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_chars(pathstr, current_position, i);
			if (!*path)
				_strcat(path, cmd);
			else
			{
				_strcat(path, "/");
				_strcat(path, cmd);
			}
			if (is_cmd(shell_info, path))
				return (path);
			if (!pathstr[i])
				break;
			current_position = i;
		}
		i++;
	}
	return (NULL);
}
