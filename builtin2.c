/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:58:28 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 15:58:41 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	update_env(const char *k, const char *v, t_env *e)
{
	int		idx;
	char	*ent;
	char	**ne;

	idx = env_index(e->g_env, k);
	ent = make_env_entry(k, v);
	if (idx >= 0)
	{
		free(e->g_env[idx]);
		e->g_env[idx] = ent;
	}
	else
	{
		ne = extend_env_array(e->g_env, ent);
		free(e->g_env);
		e->g_env = ne;
	}
	e->environ = e->g_env;
}

void	remove_env(const char *k, t_env *e)
{
	int		idx;
	char	**ne;

	idx = env_index(e->g_env, k);
	if (idx < 0)
		return ;
	free(e->g_env[idx]);
	ne = delete_env_array(e->g_env, idx);
	free(e->g_env);
	e->g_env = ne;
	e->environ = e->g_env;
}

char	escape_char(char c)
{
	if (c == 'n')
		return ('\n');
	if (c == 't')
		return ('\t');
	if (c == 'r')
		return ('\r');
	if (c == '\\')
		return ('\\');
	return (c);
}

char	next_escaped_char(const char *s, size_t *i)
{
	char	c;

	if (s[*i] == '\\' && s[*i + 1])
	{
		*i += 1;
		c = escape_char(s[*i]);
		*i += 1;
	}
	else
	{
		c = s[*i];
		*i += 1;
	}
	return (c);
}

char	*interpret_escapes(const char *s)
{
	size_t	i;
	size_t	j;
	size_t	n;
	char	*b;
	char	c;

	i = 0;
	j = 0;
	n = ft_strlen(s) * 2 + 1;
	b = malloc(n);
	if (!b)
		fatal_error("malloc failed in interpret_escapes");
	while (s[i])
	{
		c = next_escaped_char(s, &i);
		b[j++] = c;
	}
	b[j] = '\0';
	return (b);
}
