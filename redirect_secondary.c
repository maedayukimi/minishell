/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_secondary.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:22:56 by mawako            #+#    #+#             */
/*   Updated: 2025/05/07 19:23:04 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sp_stdout_stdout(t_redirect *r)
{
	r->type = RD_SP;
	r->targetfd = STDOUT;
	r->filefd = STDOUT;
}

void	handle_sp_stdout_stderr(t_redirect *r)
{
	r->type = RD_SP;
	r->targetfd = STDOUT;
	r->filefd = STDERR;
}

void	handle_sp_stderr_stdout(t_redirect *r)
{
	r->type = RD_SP;
	r->targetfd = STDERR;
	r->filefd = STDOUT;
}

void	handle_sp_stderr_stderr(t_redirect *r)
{
	r->type = RD_SP;
	r->targetfd = STDERR;
	r->filefd = STDERR;
}
