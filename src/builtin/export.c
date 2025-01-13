/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 12:43:38 by saylital          #+#    #+#             */
/*   Updated: 2025/01/13 19:03:18 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	env_alphabetic(t_ms *shell)
{
	int		i;
	int		size;
	char	**copy_list;

	i = 0;
	size = env_list_size(shell->env_list);
	copy_list = malloc((size + 1) * sizeof(char *));
	//handle malloc fail and free stuff
	while (shell->env_list[i])
	{
		copy_list[i] = shell->env_list[i];
		i++;
	}
	copy_list[i] = NULL;
	sort_aplhabetically();
}

void	ft_export(char **command, t_ms *shell)
{
	int	i;

	i = 0;
	if (!command[1])
	{
		//this probably need to be custom function and printed alphabetically
		ft_env(command, shell);
		return ;
	}


}