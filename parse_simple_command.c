/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_simple_command.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:16:52 by mawako            #+#    #+#             */
/*   Updated: 2025/05/15 16:50:25 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	handle_fd_dup(t_token **tok, t_node *node)
{
	t_redirect	*redir;

	redir = redirect_type2(tok, *tok, (*tok)->word);
	if (!redir)
	{
		fprintf(stderr,
			"minishell: syntax error with redirection operator '%s'\n",
			(*tok)->word);
		free_node(node);
		return (0);
	}
	append_redirect(&node->redirects, redir);
	return (1);
}

static int	handle_redirect(t_token **tok, t_node *node)
{
	if (!(*tok)->next || (*tok)->next->kind != TK_WORD)
	{
		fprintf(stderr,
			"minishell: syntax error near unexpected token `newline'\n");
		free_node(node);
		return (0);
	}
	append_redirect(&node->redirects,
		redirect_type(tok, *tok, (*tok)->word));
	return (1);
}

static int	process_simple_token(t_token **tok, t_node *node)
{
	if ((*tok)->kind == TK_WORD)
	{
		append_tok(&node->args, tokdup(*tok));
		*tok = (*tok)->next;
	}
	else if (is_redirect_op(*tok))
	{
		if (is_fd_dup_op((*tok)->word))
			return (handle_fd_dup(tok, node));
		return (handle_redirect(tok, node));
	}
	else
		fatal_error("parse_simple_command: unexpected token");
	return (1);
}

t_node	*parse_simple_command(t_token **rest)
{
	t_node	*node;
	t_token	*tok;

	node = new_node(ND_SIMPLE_CMD);
	tok = *rest;
	while (tok && tok->kind != TK_EOF
		&& !is_control_op(tok)
		&& !(tok->kind == TK_OP
			&& strcmp(tok->word, ")") == 0))
	{
		if (!process_simple_token(&tok, node))
			return (NULL);
	}
	*rest = tok;
	return (node);
}
