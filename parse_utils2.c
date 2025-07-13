/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:30:31 by shuu              #+#    #+#             */
/*   Updated: 2025/05/21 16:30:54 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_fd_dup_op(const char *op)
{
	if (!op)
		return (0);
	return (!ft_strcmp(op, ">&1")
		|| !ft_strcmp(op, "1>&1")
		|| !ft_strcmp(op, ">&2")
		|| !ft_strcmp(op, "1>&2")
		|| !ft_strcmp(op, "2>&1")
		|| !ft_strcmp(op, "2>&2"));
}

int	is_redirect_op(t_token *tok)
{
	static char *const	redirs[] = {
		"1>>", "2>>", ">>", "<<", "<", ">", "2>", "1>",
		">&1", "1>&1", "1>&2", ">&2", "2>&2", "2>&1", "&>", ">&", NULL
	};
	int					i;

	if (!tok || tok->kind != TK_OP)
		return (0);
	i = 0;
	while (redirs[i])
		if (!ft_strcmp(tok->word, redirs[i++]))
			return (1);
	return (0);
}

int	is_control_op(t_token *tok)
{
	if (!tok || tok->kind != TK_OP)
		return (0);
	return (!ft_strcmp(tok->word, ";")
		|| !ft_strcmp(tok->word, "&&")
		|| !ft_strcmp(tok->word, "||")
		|| !ft_strcmp(tok->word, "|")
		|| !ft_strcmp(tok->word, "&"));
}
