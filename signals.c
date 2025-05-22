/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:20:13 by mawako            #+#    #+#             */
/*   Updated: 2025/05/22 21:12:00 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_sig_subshell;

void	sigint_handler(int signum)
{
	if (g_sig_subshell)
	{
		ft_dprintf(STDERR_FILENO, "minishell
			: syntax error near unexpected token `newline'\n",
			NULL, NULL);
		exit(1);
	}
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigchld_handler(int sig)
{
	(void)sig;
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	flush_stdin();
}

void	flush_stdin(void)
{
	int		n;
	char	buf[256];
	int		r;
	int		to_read;

	if (ioctl(STDIN_FILENO, FIONREAD, &n) == 0 && n > 0)
	{
		while (n > 0)
		{
			if (n < 256)
				to_read = n;
			else
				to_read = 256;
			r = read(STDIN_FILENO, buf, to_read);
			if (r <= 0)
				break ;
			n -= r;
		}
	}
}
