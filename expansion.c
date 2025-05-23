/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:27 by mawako            #+#    #+#             */
/*   Updated: 2025/05/23 18:05:01 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	append_char(char **s, char c)
{
	char	*new;
	size_t	size;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
	if (!new)
		fatal_error("append_char: malloc");
	if (*s)
	{
		strlcpy(new, *s, size);
		free(*s);
	}
	new[size - 2] = c;
	new[size - 1] = '\0';
	*s = new;
}

char	*find_env(char *p)
{
	char	*env;
	char	*content;
	size_t	size;

	p++;
	size = 0;
	while (p[size] && (p[size] != DUOBLE_QUOTE_CHAR)
		&& (p[size] != SINGLE_QUOTE_CHAR) && !is_blank(p[size]))
		size++;
	env = strndup(p, size);
	if (!env)
		fatal_error("find_env: strndup");
	content = getenv(env);
	free(env);
	if (!content)
		return (NULL);
	return (content);
}

void	quote_removal(t_node *node, t_env *env)
{
	if (!node)
		return ;
	if (node->kind == ND_SUBSHELL && node->child)
		quote_removal(node->child, env);
	else
	{
		if (node->args)
			remove_quote(node->args, env);
		if (node->filename)
			remove_quote(node->filename, env);
		quote_removal_redirects(node->redirects);
	}
	if (node->next)
		quote_removal(node->next, env);
}

void	quote_removal_redirects(t_redirect *redir)
{
	char	*new_word;

	while (redir)
	{
		if (redir->word)
		{
			new_word = remove_quote_word(redir->word);
			redir->word = new_word;
		}
		redir = redir->next;
	}
}

void	expansion(t_node *node, t_env *env)
{
	quote_removal(node, env);
}
