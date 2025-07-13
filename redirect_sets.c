/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_sets.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:23:44 by mawako            #+#    #+#             */
/*   Updated: 2025/05/07 19:23:51 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	set_redir_type(t_redirect *r, char *type)
{
	if (ft_strcmp(type, "<<") == 0)
		handle_rd_heredoc(r);
	else if (ft_strcmp(type, "<") == 0)
		handle_rd_in(r);
	else if (ft_strcmp(type, ">") == 0)
		handle_rd_out_stdout(r);
	else if (ft_strcmp(type, "1>") == 0)
		handle_rd_out_fd1(r);
	else if (ft_strcmp(type, ">>") == 0)
		handle_rd_append_stdout(r);
	else if (ft_strcmp(type, "1>>") == 0)
		handle_rd_append_fd1(r);
	else if (ft_strcmp(type, "2>") == 0)
		handle_rd_err_stdout(r);
	else if (ft_strcmp(type, "2>>") == 0)
		handle_rd_err_append(r);
	else
		fatal_error("redirect_type: unknown type");
}

int	set_redir2_type(t_redirect *r, char *type)
{
	if (ft_strcmp(type, ">&1") == 0 || ft_strcmp(type, "1>&1") == 0)
		handle_sp_stdout_stdout(r);
	else if (ft_strcmp(type, ">&2") == 0 || ft_strcmp(type, "1>&2") == 0)
		handle_sp_stdout_stderr(r);
	else if (ft_strcmp(type, "2>&1") == 0)
		handle_sp_stderr_stdout(r);
	else if (ft_strcmp(type, "2>&2") == 0)
		handle_sp_stderr_stderr(r);
	else
		return (0);
	return (1);
}
