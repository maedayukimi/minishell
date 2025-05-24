/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_open.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 20:46:37 by shuu              #+#    #+#             */
/*   Updated: 2025/05/24 20:50:10 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_flags_fd(t_redirect *redir, int flags, mode_t mode)
{
	if (redir->type == RD_OUT || redir->type
		== RD_OUT_ERR || redir->type == RD_SP)
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	else if (redir->type == RD_IN || redir->type == RD_HEREDOC)
		flags = O_RDONLY;
	else if (redir->type == RD_APPEND_OUT)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else if (redir->type == RD_APPEND_IN)
		flags = O_RDONLY | O_APPEND;
	if (redir->type == RD_HEREDOC)
		redir->filefd = open(redir->heredoc_filename, O_RDONLY);
	else if (redir->word)
		redir->filefd = open(redir->word, flags, mode);
}
