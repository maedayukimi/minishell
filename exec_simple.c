/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:21:22 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 21:50:49 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_subshell(t_node **node_ptr, t_env *env)
{
	int		status;
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid < 0)
		fatal_error("fork failed");
	if (pid == 0)
	{
		status = exec_tree((*node_ptr)->child, env);
		exit(status);
	}
	else
	{
		waitpid(pid, &wstatus, 0);
		if (WIFEXITED(wstatus))
			status = WEXITSTATUS(wstatus);
		else
			status = 1;
	}
	*node_ptr = (*node_ptr)->next;
	return (status);
}

int	handle_logical_separator(t_node **node, int status)
{
	if ((*node)->separator)
	{
		if (strcmp((*node)->separator, "&&") == 0)
		{
			if (status != 0)
				return (1);
		}
		else if (strcmp((*node)->separator, "||") == 0)
		{
			if (status == 0)
				return (1);
		}
	}
	*node = (*node)->next;
	return (0);
}

int	handle_bg(t_node **node_ptr, t_env *env)
{
	int	status;

	status = exec_background(*node_ptr, env);
	*node_ptr = (*node_ptr)->next;
	return (status);
}

int	exec(t_node *node, t_env *env)
{
	int	status;

	status = 0;
	while (node)
	{
		if (node->kind == ND_SUBSHELL)
			status = handle_subshell(&node, env);
		else if (node->separator && strcmp(node->separator, "|") == 0)
			status = handle_pl(&node, env);
		else if (node->separator && strcmp(node->separator, "&") == 0)
			status = handle_bg(&node, env);
		else
		{
			status = exec_cmd(node, env);
			if (handle_logical_separator(&node, status))
				break ;
		}
	}
	return (status);
}
