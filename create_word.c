/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:04:39 by mawako            #+#    #+#             */
/*   Updated: 2025/07/13 15:14:54 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		fatal_error("new_token: malloc");
	tok->word = word;
	tok->kind = kind;
	tok->next = NULL;
	return (tok);
}

t_token	*operator_func(char **rest, char *line)
{
	size_t				i;
	char				*op;
	static char *const	operators[] = {
		"1>&1", "1>&2", ">&1", "2>&2", ">&2", "2>&1", "2>", "2>>", "1>", "1>>",
		"&>", "&>>", ">>", "<<", ">", "<", "||", "&&", "&", ";", ";;", "(",
		")", "|", "\n", NULL
	};

	i = 0;
	while (operators[i])
	{
		if (startswith(line, operators[i]))
		{
			op = ft_strdup(operators[i]);
			if (!op)
				fatal_error("operator_func: ft_strdup");
			*rest = line + ft_strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	fatal_error("Unexpected operator");
	return (NULL);
}

static size_t	get_word_len(char *line)
{
	size_t	len;
	int		in_squote;
	int		in_dquote;

	in_squote = 0;
	in_dquote = 0;
	len = 0;
	while (line[len])
	{
		if (line[len] == '\'' && !in_dquote)
			in_squote = !in_squote;
		else if (line[len] == '"' && !in_squote)
			in_dquote = !in_dquote;
		if (!in_squote && !in_dquote)
		{
			if (is_blank(line[len]) || is_metacharacter(line[len])
				|| check_redirect(line + len))
				break ;
		}
		len++;
	}
	return (len);
}

t_token	*word_func(char **rest, char *line)
{
	size_t	len;
	char	*tmp;

	len = get_word_len(line);
	tmp = strndup(line, len);
	if (!tmp)
		fatal_error("word_func: strndup");
	*rest = line + len;
	return (new_token(tmp, TK_WORD));
}
