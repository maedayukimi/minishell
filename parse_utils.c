/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:46:54 by mawako            #+#    #+#             */
/*   Updated: 2025/05/20 18:02:50 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char *const	g_redirs[] = {
	"1>>", "2>>", ">>", "<<", "<", ">", "2>", "1>",
	">&1", "1>&1", "1>&2", ">&2", "2>&2", "2>&1", "&>", ">&", NULL
};

int	is_fd_dup_op(const char *op)
{
	if (!op)
		return (0);
	return (!strcmp(op, ">&1")
		|| !strcmp(op, "1>&1")
		|| !strcmp(op, ">&2")
		|| !strcmp(op, "1>&2")
		|| !strcmp(op, "2>&1")
		|| !strcmp(op, "2>&2"));
}

int	is_redirect_op(t_token *tok)
{
	int	i;

	i = 0;
	if (!tok || tok->kind != TK_OP)
		return (0);
	while (g_redirs[i])
		if (!strcmp(tok->word, g_redirs[i++]))
			return (1);
	return (0);
}

int	is_control_op(t_token *tok)
{
	if (!tok || tok->kind != TK_OP)
		return (0);
	return (!strcmp(tok->word, ";")
		|| !strcmp(tok->word, "&&")
		|| !strcmp(tok->word, "||")
		|| !strcmp(tok->word, "|")
		|| !strcmp(tok->word, "&"));
}

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
