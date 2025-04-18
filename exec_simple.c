/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_simple.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:21:22 by mawako            #+#    #+#             */
/*   Updated: 2025/04/11 17:09:10 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	handle_subshell(t_node **node_ptr)
{
	int		status;
	pid_t	pid;
	int		wstatus;

	pid = fork();
	if (pid < 0)
		fatal_error("fork failed");
	if (pid == 0)
	{
		status = exec_tree((*node_ptr)->child);
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

int	handle_pl(t_node **node_ptr)
{
	int		status;
	t_node	*pipeline_head;
	t_node	*cur;
	t_node	*next_ptr;

	pipeline_head = *node_ptr;
	cur = pipeline_head;
	while (cur && cur->separator
		&& strcmp(cur->separator, "|") == 0 && cur->next)
		cur = cur->next;
	if (cur)
		next_ptr = cur->next;
	else
		next_ptr = NULL;
	if (cur)
		cur->next = NULL;
	status = exec_pl(pipeline_head);
	if (cur)
		cur->next = next_ptr;
	*node_ptr = next_ptr;
	return (status);
}

int	handle_bg(t_node **node_ptr)
{
	int	status;

	status = exec_background(*node_ptr);
	*node_ptr = (*node_ptr)->next;
	return (status);
}

int	exec(t_node *node)
{
	int	status;

	status = 0;
	while (node)
	{
		if (node->kind == ND_SUBSHELL)
			status = handle_subshell(&node);
		else if (node->separator && strcmp(node->separator, "|") == 0)
			status = handle_pl(&node);
		else if (node->separator && strcmp(node->separator, "&") == 0)
			status = handle_bg(&node);
		else
		{
			status = exec_cmd(node);
			if (handle_logical_separator(&node, status))
				break ;
		}
	}
	return (status);
}
