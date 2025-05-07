/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_primary2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:22:27 by mawako            #+#    #+#             */
/*   Updated: 2025/05/07 19:22:35 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_rd_append_stdout(t_redirect *r)
{
	r->type = RD_APPEND_OUT;
	r->targetfd = STDOUT;
}

void	handle_rd_append_fd1(t_redirect *r)
{
	r->type = RD_APPEND_OUT;
	r->targetfd = STDOUT;
}

void	handle_rd_err_stdout(t_redirect *r)
{
	r->type = RD_OUT;
	r->targetfd = STDERR;
}

void	handle_rd_err_append(t_redirect *r)
{
	r->type = RD_APPEND_OUT;
	r->targetfd = STDERR;
}
