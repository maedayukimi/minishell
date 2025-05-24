/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_reset.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:45:03 by shuu              #+#    #+#             */
/*   Updated: 2025/05/24 20:53:22 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	reset_in_out_append_heredoc(t_redirect *redir)
{
	close(redir->filefd);
	dup2(redir->stashed_targetfd, redir->targetfd);
	close(redir->stashed_targetfd);
}

void	reset_sp(t_redirect *redir)
{
	close(redir->filefd);
	dup2(redir->stashed_targetfd, redir->targetfd);
	close(redir->stashed_targetfd);
}

void	reset_out_err(t_redirect *redir)
{
	close(redir->filefd);
	dup2(redir->stashed_targetfd, redir->targetfd);
	dup2(redir->stashed_targetfd2, redir->targetfd2);
	close(redir->stashed_targetfd);
	close(redir->stashed_targetfd2);
}
