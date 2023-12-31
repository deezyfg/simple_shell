#include "shell.h"

/**
 * _erratoi - Converts a string to an integer.
 *
 * @str: The string to be converted.
 *
 * Return:
 * - If successful: The converted number .
 * - If no numbers in the string: 0
 * - On error: -1
 */
int _erratoi(char *str)
{
	int i = 0;
	unsigned long int result = 0;

	if (*str == '+')
		str++;  /* TODO: Why this causes the main function to return 255.*/
	for (i = 0;  str[i] != '\0'; i++)
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			result *= 10;
			result += (str[i] - '0');
			if (result > INT_MAX)
				return (-1);
		}
		else
			return (-1);
	}
	return (result);
}

/**
 * print_error - Prints an error message.
 * @shell_info: Pointer to structure with potential arguments.
 * @estr: String containing specified error type.
 *
 * Return:
 * - If successful: The converted number .
 * - If no numbers in the string: 0
 * - On error: -1
 */

void print_error(info_t *shell_info, char *estr)
{
	_eputs(shell_info->fname);
	_eputs(": ");
	print_d(shell_info->line_count, STDERR_FILENO);
	_eputs(": ");
	_eputs(shell_info->argv[0]);
	_eputs(": ");
	_eputs(estr);
}

/**
 * print_d - Prints a decimal (base 10) integer.
 * @input: The input number.
 * @fd: The file descriptor to write to.
 *
 * Return: Number of characters printed.
 */

int print_d(int input, int fd)
{
	int (*__putchar)(char) = _putchar;
	int i, count = 0;
	unsigned int _abs_, current;

	if (fd == STDERR_FILENO)
		__putchar = _eputchar;
	if (input < 0)
	{
		_abs_ = -input;
		__putchar('-');
		count++;
	}
	else
		_abs_ = input;
	current = _abs_;
	for (i = 1000000000; i > 1; i /= 10)
	{
		if (_abs_ / i)
		{
			__putchar('0' + current / i);
			count++;
		}
		current %= i;
	}
	__putchar('0' + current);
	count++;

	return (count);
}

/**
 * convert_number - Converts a number to a string.
 * @num: Number to convert.
 * @base: Base for conversion.
 * @flags: Argument flags.
 *
 * Return: The converted string.
 */

char *convert_number(long int num, int base, int flags)
{
	static char *array;
	static char buffer[50];
	char sign = 0;
	char *ptr;
	unsigned long n = num;

	if (!(flags & CONVERT_UNSIGNED) && num < 0)
	{
		n = -num;
		sign = '-';

	}
	array = flags & CONVERT_LOWERCASE ? "0123456789abcdef" : "0123456789ABCDEF";
	ptr = &buffer[49];
	*ptr = '\0';

	do	{
		*--ptr = array[n % base];
		n /= base;
	} while (n != 0);

	if (sign)
		*--ptr = sign;
	return (ptr);
}

/**
 * remove_comments - Replaces the first instance of '#' with '\0'.
 * @buf: Address of the string to modify.
 *
 * Return: Always 0;
 */

void remove_comments(char *buf)
{
	int i;

	for (i = 0; buf[i] != '\0'; i++)
		if (buf[i] == '#' && (!i || buf[i - 1] == ' '))
		{
			buf[i] = '\0';
			break;
		}
}
