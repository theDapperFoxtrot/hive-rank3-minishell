/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smishos <smishos@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/19 20:09:36 by smishos           #+#    #+#             */
/*   Updated: 2025/03/19 20:09:37 by smishos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	check_signals(int signbr, void (*handler_function)(int))
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_handler = handler_function;
	sa.sa_flags = 0;
	if (sigaction(signbr, &sa, NULL) < 0)
	{
		ft_putendl_fd("Error: sigaction failed", 2);
		return (1);
	}
	return (0);
}

int	init_signals(void)
{
	if (check_signals(SIGINT, sig_handler_sigint) == 1)
		return (1);
	if (check_signals(SIGQUIT, SIG_IGN) == 1)
		return (1);
	return (0);
}

int	default_signals(void)
{
	if (signal(SIGINT, SIG_DFL) == SIG_ERR)
		return (1);
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		return (1);
	return (0);
}
