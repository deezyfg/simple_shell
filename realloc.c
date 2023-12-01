#include "shell.h"

/**
 * _memset - Fills memory with a constant byte.
 *
 * @str: Pointer to the memory area.
 * @b: The byte to fill *str with.
 * @n: The number of bytes to be filled.
 *
 * Return: A pointer to the memory area str.
 */
char *_memset(char *str, char b, unsigned int n)
{
	unsigned int i;

	for (i = 0; i < n; i++)
		str[i] = b;
	return (str);
}

/**
 * ffree - Frees a string of strings.
 *
 * @str_array: String array to be freed.
 */
void ffree(char **str_array)
{
	char **current_str = str_array;

	if (!str_array)
		return;
	while (*str_array)
		free(*str_array++);
	free(current_str);
}

/**
 * _realloc - Reallocates a block of memory.
 *
 * @ptr: Pointer to the previous malloc'ated block.
 * @old_size: Byte size of the previous block.
 * @new_size: Byte size of the new block.
 *
 * Return: A pointer to the reallocated block.
 */
void *_realloc(void *ptr, unsigned int old_size, unsigned int new_size)
{
	char *new_ptr;

	if (!ptr)
		return (malloc(new_size));
	if (!new_size)
		return (free(ptr), NULL);
	if (new_size == old_size)
		return (ptr);

	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);

	old_size = old_size < new_size ? old_size : new_size;
	while (old_size--)
		new_ptr[old_size] = ((char *)ptr)[old_size];
	free(ptr);
	return (new_ptr);
}