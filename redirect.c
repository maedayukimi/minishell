/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:46 by mawako            #+#    #+#             */
/*   Updated: 2025/05/24 20:58:04 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	open_redir_file(t_redirect *redir)
{
	int		flags;
	mode_t	mode;

	while (redir)
	{
		flags = 0;
		mode = 0644;
		set_flags_fd(redir, flags, mode);
		if (redir->filefd < 0)
		{
			ft_dprintf(STDERR_FILENO, "minishell: %s: %s\n",
				redir->word, strerror(errno));
			return (-1);
		}
		redir = redir->next;
	}
	return (0);
}

void	do_redirect(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == RD_OUT
			|| redir->type == RD_IN
			|| redir->type == RD_APPEND_OUT)
			do_out_in_append_out(redir);
		else if (redir->type == RD_SP && !redir->next)
			do_sp(redir);
		else if (redir->type == RD_OUT_ERR)
			do_out_err(redir);
		else if (redir->type == RD_HEREDOC)
			do_heredoc(redir);
		redir = redir->next;
	}
}

void	reset_redirect(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == RD_IN || redir->type == RD_OUT
			|| redir->type == RD_APPEND_OUT || redir->type == RD_APPEND_IN
			|| redir->type == RD_HEREDOC)
			reset_in_out_append_heredoc(redir);
		else if (redir->type == RD_SP && !redir->next)
			reset_sp(redir);
		else if (redir->type == RD_OUT_ERR && redir->filefd >= 0)
			reset_out_err(redir);
		redir = redir->next;
	}
}
