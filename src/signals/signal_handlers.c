#include "../../include/minishell.h"

void	sig_handler_child(int signal)
{
	if (signal == SIGINT)
    {
        g_signal = signal;
        write(2, "\n", 2);
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
        g_signal = signal;
        write(2, "\n", 2);
    }
}

void sig_handler_heredoc(int signal)
{
    if (signal == SIGINT)
    {
        g_signal = signal;
        write(2, "\n", 2);
        rl_replace_line("", 0);
        rl_on_new_line();
        rl_done = 1;
    }
}
