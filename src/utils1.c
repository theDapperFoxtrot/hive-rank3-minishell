#include "../include/minishell.h"

char	*free_and_nullify(char *str_ptr)
{
	free(str_ptr);
	str_ptr = NULL;
	return (str_ptr);
}

int	validate_str(char *str, char *acc_values)
{
	size_t	s_i;
	size_t	a_i;

	s_i = 0;
	if (str == NULL || acc_values == NULL)
		return (-1);
	while (str[s_i] != '\0')
	{
		a_i = 0;
		while (acc_values[a_i] != '\0')
		{
			if (str[s_i] == acc_values[a_i])
				break ;
			a_i++;
			if (acc_values[a_i] == '\0')
				return (-1);
		}
		s_i++;
	}
	return (1);
}

char	*count_for_malloc(char *str)
{
	int		i;
	int		j;
	char	*parsed;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] != 34 && str[i] != 39)
			j++;
		i++;
	}
	parsed = malloc((j + 1) * sizeof(char));
	return (parsed);
}

int	event(void)
{
	return (0);
}
