/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_sep.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 15:27:28 by shuu              #+#    #+#             */
/*   Updated: 2025/05/24 22:43:18 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	get_logical_sep(t_node **node_ptr, char **logical_sep)
{
	t_node	*tail;
	int		status;

	status = 0;
	tail = *node_ptr;
	while (tail->separator && !ft_strcmp(tail->separator, "|"))
	{
		if (!tail->next)
		{
			ft_dprintf(STDERR_FILENO,
				"minishell: syntax error near unexpected token `|'\n",
				NULL, NULL);
			*node_ptr = NULL;
			return (258);
		}
		tail = tail->next;
	}
	*logical_sep = tail->separator;
	return (status);
}

int	process_exec_pl(t_node **node_ptr, int *exit_loop, t_env *env)
{
	char		*logical_sep;
	int			status;

	status = 0;
	logical_sep = NULL;
	if (!(*node_ptr)->next)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error near unexpected token `|'\n",
			NULL, NULL);
		*node_ptr = NULL;
		return (258);
	}
	status = get_logical_sep(node_ptr, &logical_sep);
	if (status == 258)
		return (258);
	status = handle_pl(node_ptr, env);
	if (logical_sep)
	{
		if (!ft_strcmp(logical_sep, "&&") && status != 0)
			*exit_loop = 1;
		else if (!ft_strcmp(logical_sep, "||") && status == 0)
			*exit_loop = 1;
	}
	return (status);
}

int	process_exec_sub(t_node **node_ptr, int *exit_loop, t_env *env)
{
	t_node	*node;
	char	*sep;
	int		status;

	node = *node_ptr;
	status = 0;
	sep = node->separator;
	status = handle_subshell(node_ptr, env);
	if (sep)
	{
		if (!ft_strcmp(sep, "&&") && status != 0)
			*exit_loop = 1;
		else if (!ft_strcmp(sep, "||") && status == 0)
			*exit_loop = 1;
	}
	return (status);
}
