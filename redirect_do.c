/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_do.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:44:16 by shuu              #+#    #+#             */
/*   Updated: 2025/05/24 20:52:32 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	do_out_in_append_out(t_redirect *redir)
{
	redir->stashed_targetfd = dup(redir->targetfd);
	dup2(redir->filefd, redir->targetfd);
}

void	do_sp(t_redirect *redir)
{
	redir->stashed_targetfd = dup(redir->targetfd);
	dup2(redir->filefd, redir->targetfd);
}

void	do_out_err(t_redirect *redir)
{
	redir->stashed_targetfd = dup(redir->targetfd);
	redir->stashed_targetfd2 = dup(redir->targetfd2);
	dup2(redir->filefd, redir->targetfd);
	dup2(redir->filefd, redir->targetfd2);
}

void	do_heredoc(t_redirect *redir)
{
	redir->stashed_targetfd = dup(redir->targetfd);
	dup2(redir->filefd, redir->targetfd);
}
