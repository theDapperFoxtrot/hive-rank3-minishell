#include "../../include/minishell.h"

void	sig_handler_child(int signal)
{
	if (signal == SIGINT)
    {
        g_signal = signal;
        write(1, "\n", 1);
    }
	if (signal == SIGQUIT)
    {
        g_signal = signal;
		ft_putstr_fd("Quit (core dumped)\n", 2);
    }
}

void sig_handler_sigint(int signal)
{
    if (signal == SIGINT)
    {
        g_signal = SIGINT;
        ft_putstr_fd("\n", STDOUT_FILENO);
        rl_on_new_line();
        rl_replace_line("", 0);
        rl_redisplay();
    }
}

void sig_handler_heredoc(int signal)
{
    if (signal == SIGINT)
    {
        g_signal = SIGINT;
        rl_replace_line("\n", 0);
        rl_done = 1;
    }
}
