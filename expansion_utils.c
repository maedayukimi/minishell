/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/20 22:19:05 by shuu              #+#    #+#             */
/*   Updated: 2025/07/01 21:42:11 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	remove_quote_single(char *p, int *i, char **new_word)
{
	(*i)++;
	while (p[*i] && p[*i] != SINGLE_QUOTE_CHAR)
		append_char(new_word, p[(*i)++]);
	if (p[*i] == SINGLE_QUOTE_CHAR)
		(*i)++;
}

static void	remove_quote_double(char *p, int *i, char **new_word, t_env *env)
{
	(*i)++;
	while (p[*i] && p[*i] != DUOBLE_QUOTE_CHAR)
	{
		if (p[*i] == '\\' && (p[(*i) + 1] == '\\' || p[(*i) + 1]
				== '"' || p[(*i) + 1] == '$' || p[(*i) + 1] == '`'))
		{
			append_char(new_word, p[(*i) + 1]);
			i += 2;
		}
		else if (p[*i] == '$')
			handle_dollar(p, i, new_word, env);
		else
			append_char(new_word, p[(*i)++]);
	}
	if (p[*i] == DUOBLE_QUOTE_CHAR)
		(*i)++;
}

static void	remove_quote_bs(char *p, int *i, char **new_word)
{
	if (p[(*i) + 1])
	{
		append_char(new_word, p[(*i) + 1]);
		(*i) += 2;
	}
	else
		(*i)++;
}

void	remove_quote(t_token *tok, t_env *env)
{
	char	*new_word;
	char	*p;
	int		i;

	if (!tok || tok->kind != TK_WORD || !tok->word)
		return ;
	new_word = ft_strdup("");
	p = tok->word;
	i = 0;
	while (p[i])
	{
		if (p[i] == SINGLE_QUOTE_CHAR)
			remove_quote_single(p, &i, &new_word);
		else if (p[i] == DUOBLE_QUOTE_CHAR)
			remove_quote_double(p, &i, &new_word, env);
		else if (p[i] == '\\')
			remove_quote_bs(p, &i, &new_word);
		else if (p[i] == '$')
			handle_dollar(p, &i, &new_word, env);
		else
			append_char(&new_word, p[i++]);
	}
	free(tok->word);
	tok->word = new_word;
	remove_quote(tok->next, env);
}
