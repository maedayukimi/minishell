/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:42:10 by mawako            #+#    #+#             */
/*   Updated: 2025/04/17 18:50:07 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_exec_node(t_node **node_ptr, int *exit_loop)
{
	int		status;
	t_node	*cur;
	char	*sep;

	*exit_loop = 0;
	if ((*node_ptr)->kind == ND_SUBSHELL)
	{
		cur = *node_ptr;
		sep = cur->separator;
		status = handle_subshell(node_ptr);
		if (sep)
		{
			if (!strcmp(sep, "&&") && status != 0)
				*exit_loop = 1;
			else if (!strcmp(sep, "||") && status == 0)
				*exit_loop = 1;
		}
	}
	else if ((*node_ptr)->separator
		&& !strcmp((*node_ptr)->separator, "|"))
		status = handle_pl(node_ptr);
	else if ((*node_ptr)->separator
		&& !strcmp((*node_ptr)->separator, "&"))
		status = handle_bg(node_ptr);
	else
	{
		status = exec_cmd(*node_ptr);
		if (handle_logical_separator(node_ptr, status))
			*exit_loop = 1;
	}
	return (status);
}

int	exec_tree(t_node *node)
{
	int	status;
	int	exit_loop;

	status = 0;
	exit_loop = 0;
	while (node)
	{
		status = process_exec_node(&node, &exit_loop);
		if (exit_loop)
			break ;
	}
	return (status);
}
