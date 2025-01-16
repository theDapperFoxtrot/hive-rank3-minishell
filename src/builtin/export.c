/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:43:38 by saylital          #+#    #+#             */
/*   Updated: 2025/01/16 17:29:37 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <stdlib.h>
#include <string.h>

// Function to compare two strings alphabetically
int compare(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

// Function to sort and print environment variables alphabetically
void print_sorted_env(t_ms *shell)
{
    int i, size;
    char **copy_list;

    size = env_list_size(shell->env_list);
    copy_list = malloc((size + 1) * sizeof(char *));
    if (!copy_list)
        return; // handle malloc failure

    for (i = 0; i < size; i++)
        copy_list[i] = shell->env_list[i];
    copy_list[size] = NULL;

    qsort(copy_list, size, sizeof(char *), compare);

    for (i = 0; copy_list[i]; i++)
        printf("%s\n", copy_list[i]);

    free(copy_list);
}

void set_env_variable(t_ms *shell, char *key, char *value)
{
	int i;

	shell->env_list = realloc(shell->env_list, (env_list_size(shell->env_list) + 2) * sizeof(char *));
	i = 0;
	while (shell->env_list[i])
	{
		if (ft_strncmp(shell->env_list[i], key, strlen(key)) == 0)
		{
			free(shell->env_list[i]);
			shell->env_list[i] = ft_strjoin(key, value);
			return ;
		}
		i++;
	}
}

void export_error(char *arg)
{
	ft_putstr_fd("export: `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

void ft_export(char **command, t_ms *shell)
{
	char *arg;
	char *equal_sign;
	char *key;
	char *value;
	int i;

    if (!command[1])
    {
        print_sorted_env(shell);
        return;
    }
	i = 1;
	while (command[i])
    {
        arg = command[i];
        equal_sign = strchr(arg, '=');
        if (equal_sign)
        {
            *equal_sign = '\0';
            key = arg;
            value = equal_sign + 1;
            set_env_variable(shell, key, value);
        }
        else
            export_error(arg);
		i++;
    }
}

// single vs double quotes
// Hello '${USER}' = Hello ${USER}
// Hello "${USER}" = Hello Steve