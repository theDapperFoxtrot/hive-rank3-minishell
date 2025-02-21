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
        ioctl(STDIN_FILENO, TIOCSTI, "\n"); // Inject a newline to break readline
        write(1, "\033[1A", 4);
    }
}

void sig_handler_heredoc(int signal)
{
    if (signal == SIGINT)
    {
        g_signal = SIGINT;
        ioctl(STDIN_FILENO, TIOCSTI, "\n"); // Inject a newline to break readline
        write(1, "\033[1A", 4);
        rl_done = 1;
    }
}
