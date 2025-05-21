/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:46:54 by mawako            #+#    #+#             */
/*   Updated: 2025/05/21 16:34:19 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_redirect(t_redirect **list, t_redirect *new_redir)
{
	t_redirect	*cur;

	if (!list || !new_redir)
		return ;
	if (!*list)
		*list = new_redir;
	else
	{
		cur = *list;
		while (cur->next)
			cur = cur->next;
		cur->next = new_redir;
	}
}

void	append_node(t_node **nodes, t_node *node)
{
	if (!*nodes)
		*nodes = node;
	else
		append_node(&(*nodes)->next, node);
}

void	append_tok(t_token **tokens, t_token *tok)
{
	if (!*tokens)
		*tokens = tok;
	else
		append_tok(&(*tokens)->next, tok);
}
