#include "../../include/minishell.h"

void	sig_heredoc(void *handler_function)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_int.sa_handler = handler_function;
	sig_int.sa_flags = 0;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = SIG_IGN;
	sig_quit.sa_flags = 0;
	sigemptyset(&sig_quit.sa_mask);
	sigaction(SIGQUIT, &sig_quit, NULL);
}

void	sig_ignore(void)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_int.sa_handler = SIG_IGN;
	sig_int.sa_flags = 0;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = SIG_IGN;
	sig_quit.sa_flags = 0;
	sigemptyset(&sig_quit.sa_mask);
	sigaction(SIGQUIT, &sig_quit, NULL);
}

void	sig_child(void *handler_function)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_int.sa_handler = handler_function;
	sig_int.sa_flags = 0;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = handler_function;
	sig_quit.sa_flags = 0;
	sigemptyset(&sig_quit.sa_mask);
	sigaction(SIGQUIT, &sig_quit, NULL);
}

void	sig_init(void *handler_function)
{
	struct sigaction	sig_int;
	struct sigaction	sig_quit;

	sig_int.sa_handler = handler_function;
	sig_int.sa_flags = 0;
	sigemptyset(&sig_int.sa_mask);
	sigaction(SIGINT, &sig_int, NULL);
	sig_quit.sa_handler = SIG_IGN;
	sig_quit.sa_flags = 0;
	sigemptyset(&sig_quit.sa_mask);
	sigaction(SIGQUIT, &sig_quit, NULL);
}
