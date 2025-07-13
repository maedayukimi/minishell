/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:42:10 by mawako            #+#    #+#             */
/*   Updated: 2025/06/06 19:14:47 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	process_exec_node(t_node **node_ptr, int *exit_loop, t_env *env)
{
	t_node	*node;
	int		status;

	node = *node_ptr;
	status = 0;
	*exit_loop = 0;
	if (node->kind == ND_SUBSHELL)
		status = process_exec_sub(node_ptr, exit_loop, env);
	else if (node->separator && !ft_strcmp(node->separator, "|"))
	{
		status = process_exec_pl(node_ptr, exit_loop, env);
		env->last_exit_status = status;
	}
	else if (node->separator && !ft_strcmp(node->separator, "&"))
		status = handle_bg(node_ptr, env);
	else
	{
		status = exec_cmd(node, env);
		if (handle_logical_separator(node_ptr, status))
			*exit_loop = 1;
	}
	return (status);
}

int	exec_tree(t_node *node, t_env *env)
{
	int	status;
	int	exit_loop;

	status = 0;
	exit_loop = 0;
	while (node)
	{
		status = process_exec_node(&node, &exit_loop, env);
		if (exit_loop)
			break ;
	}
	return (status);
}
