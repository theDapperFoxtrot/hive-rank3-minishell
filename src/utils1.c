#include "../include/minishell.h"

char	*free_and_nullify(char *str_ptr)
{
	free(str_ptr);
	str_ptr = NULL;
	return (str_ptr);
}
