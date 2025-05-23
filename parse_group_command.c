/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_group_command.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:19:05 by mawako            #+#    #+#             */
/*   Updated: 2025/05/23 18:20:09 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	check_group_open(t_token **tok)
{
	if (!*tok || (*tok)->kind != TK_OP
		|| strcmp((*tok)->word, "(") != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error: expected '('\n", NULL, NULL);
		return (0);
	}
	*tok = (*tok)->next;
	return (1);
}

static int	check_group_close(t_token *tok, t_node *head)
{
	if (!tok || tok->kind == TK_EOF
		|| strcmp(tok->word, ")") != 0)
	{
		ft_dprintf(STDERR_FILENO,
			"minishell: syntax error: missing ')'\n", NULL, NULL);
		free_node(head);
		return (0);
	}
	return (1);
}

static int	process_group_token(t_token **tok, t_node **head, t_node **cur)
{
	t_node	*next;

	if ((*tok)->kind == TK_OP
		&& strcmp((*tok)->word, "(") == 0)
		next = parse_group_command(tok);
	else
		next = parse_simple_command(tok);
	if (!next)
		return (0);
	if (!*head)
		*head = next;
	else
		append_node(&(*cur)->next, next);
	*cur = next;
	if (is_control_op(*tok)
		&& !((*tok)->kind == TK_OP
			&& strcmp((*tok)->word, ")") == 0))
	{
		(*cur)->separator = strdup((*tok)->word);
		*tok = (*tok)->next;
	}
	return (1);
}

t_node	*parse_group_command(t_token **rest)
{
	t_token	*tok;
	t_node	*head;
	t_node	*cur;
	t_node	*node;

	tok = *rest;
	head = NULL;
	cur = NULL;
	if (!check_group_open(&tok))
		return (NULL);
	while (tok && tok->kind != TK_EOF
		&& !(tok->kind == TK_OP
			&& strcmp(tok->word, ")") == 0))
	{
		if (!process_group_token(&tok, &head, &cur))
			return (NULL);
	}
	if (!check_group_close(tok, head))
		return (NULL);
	*rest = tok->next;
	node = new_node(ND_SUBSHELL);
	node->child = head;
	return (node);
}
