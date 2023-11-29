#include "shell.h"

/**
 * interactive - Checks if the shell is in interactive mode.
 * @shell_info: Pointer to the info_t structure.
 *
 * Return:
 * - If the shell is in interactive mode: 1
 * - Otherwise: 0
 */
int interactive(info_t *shell_info)
{
	return (isatty(STDIN_FILENO) && shell_info->readfd <= 2);
}

/**
 * is_delim - Checks if character is a delimeter.
 * @ch: The character to check.
 * @delim_str: The delimeter string.
 *
 * Return:
 * - If the character is a delimiter: 1
 * - Otherwise: 0
 */
int is_delim(char ch, char *delim_str)
{
	while (*delim_str)
		if (*delim_str++ == ch)
			return (1);
	return (0);
}

/**
 *_isalpha - Checks if a character is alphabetic.
 *@ch: The character to check.
 *
 * Return:
 * - If the character is alphabetic: 1
 * - Otherwise: 0
 */
int _isalpha(int ch)
{
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		return (1);
	else
		return (0);
}

/**
 *_atoi - Converts a string to an integer.
 *@str: The string to be converted.
 *
 * Return:
 * - If no numbers in the string: o
 * - Otherwise: The converted number
 */
int _atoi(char *str)
{
	int i, sign = 1, flag = 0, result;
	unsigned int num = 0;

	for (i = 0;  str[i] != '\0' && flag != 2; i++)
	{
		if (str[i] == '-')
			sign *= -1;

		if (str[i] >= '0' && str[i] <= '9')
		{
			flag = 1;
			num *= 10;
			num += (str[i] - '0');
		}
		else if (flag == 1)
			flag = 2;
	}

	if (sign == -1)
		result = -num;
	else
		result = num;

	return (result);
}
