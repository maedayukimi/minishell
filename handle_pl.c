/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_pl.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 21:50:57 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 21:54:47 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static t_node	*detach_pipeline(t_node **head, t_node **next_out)
{
	t_node	*cur;

	cur = *head;
	while (cur && cur->separator && strcmp(cur->separator, "|") == 0)
		cur = cur->next;
	if (cur)
	{
		*next_out = cur->next;
		cur->next = NULL;
	}
	else
		*next_out = NULL;
	return (*head);
}

static int	report_pipe_error(void)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error near unexpected token `|'\n", NULL, NULL);
	return (258);
}

int	handle_pl(t_node **node_ptr, t_env *env)
{
	int		status;
	t_node	*pipe_head;
	t_node	*next;

	if (!(*node_ptr)->next)
		return (report_pipe_error());
	pipe_head = detach_pipeline(node_ptr, &next);
	status = exec_pl(pipe_head, env);
	*node_ptr = next;
	return (status);
}
