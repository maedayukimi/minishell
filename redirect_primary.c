/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_primary.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:21:35 by mawako            #+#    #+#             */
/*   Updated: 2025/05/07 19:21:52 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_rd_heredoc(t_redirect *r)
{
	r->type = RD_HEREDOC;
	r->targetfd = STDIN;
}

void	handle_rd_in(t_redirect *r)
{
	r->type = RD_IN;
	r->targetfd = STDIN;
}

void	handle_rd_out_stdout(t_redirect *r)
{
	r->type = RD_OUT;
	r->targetfd = STDOUT;
}

void	handle_rd_out_fd1(t_redirect *r)
{
	r->type = RD_OUT;
	r->targetfd = STDOUT;
}
