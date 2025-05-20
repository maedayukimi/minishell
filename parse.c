/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:04:59 by mawako            #+#    #+#             */
/*   Updated: 2025/05/20 18:30:54 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_redirect(const char *word)
{
	int			i;
	static char *const	redirs[] = {
		"1>>", "2>>", ">>", "<<", "<", ">", "2>", "1>", ">&1", "1>&1",
		"1>&2", ">&2", "2>&2", "2>&1", "&>", ">&", NULL
	};

	i = 0;
	if (!word)
		return (0);
	while (redirs[i])
	{
		if (!strcmp(redirs[i], word))
			return (1);
		i++;
	}
	return (0);
}

static int	process_parse_token(t_token **tok, t_node **head, t_node **cur)
{
	t_node	*next_cmd;

	if ((*tok)->kind == TK_OP && strcmp((*tok)->word, ")") == 0)
	{
		fprintf(stderr, "minishell: syntax error near unexpected token `)`\n");
		return (0);
	}
	if ((*tok)->kind == TK_OP && strcmp((*tok)->word, "(") == 0)
		next_cmd = parse_group_command(tok);
	else
		next_cmd = parse_simple_command(tok);
	if (!next_cmd)
		return (0);
	if (!*head)
		*head = next_cmd;
	else
		append_node(&(*cur)->next, next_cmd);
	*cur = next_cmd;
	if (is_control_op(*tok))
	{
		(*cur)->separator = strdup((*tok)->word);
		*tok = (*tok)->next;
	}
	return (1);
}

t_node	*parse(t_token *tok)
{
	t_node	*head;
	t_node	*cur;

	head = NULL;
	cur = NULL;
	while (tok && tok->kind != TK_EOF)
		if (!process_parse_token(&tok, &head, &cur))
			return (NULL);
	return (head);
}
