/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setenv.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:22:06 by mawako            #+#    #+#             */
/*   Updated: 2025/07/13 15:37:11 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"
#include "minishell.h"

static int	find_var_index(const char *name, t_env *env)
{
	int		i;
	size_t	len;

	if (!name || !env->g_env)
		return (-1);
	len = ft_strlen(name);
	i = 0;
	while (env->g_env[i])
	{
		if (!ft_strncmp(env->g_env[i], name, len) && env->g_env[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	*make_entry(const char *name, const char *value)
{
	char	*e;
	size_t	len;

	len = ft_strlen(name) + ft_strlen(value) + 2;
	e = malloc(len);
	if (!e)
		return (NULL);
	ft_sprintf(e, "%s=%s", name, value);
	return (e);
}

static int	expand_env(t_env *env, int new_count)
{
	char	**new_env;
	int		i;

	new_env = malloc(sizeof(char *) * (new_count + 1));
	if (!new_env)
		return (-1);
	i = 0;
	while (env->g_env && env->g_env[i])
	{
		new_env[i] = env->g_env[i];
		i++;
	}
	new_env[new_count] = NULL;
	free(env->g_env);
	env->g_env = new_env;
	return (0);
}

static int	add_entry(t_env *env, char *entry)
{
	int	count;

	count = 0;
	while (env->g_env && env->g_env[count])
		count++;
	if (expand_env(env, count + 1) < 0)
		return (-1);
	env->g_env[count] = entry;
	return (0);
}

int	my_setenv(const char *name, const char *value, int overwrite, t_env *env)
{
	int		idx;
	char	*entry;

	idx = find_var_index(name, env);
	entry = make_entry(name, value);
	if (!name || ft_strchr(name, '='))
		return (-1);
	if (idx >= 0)
	{
		if (!overwrite)
			return (0);
		if (!entry)
			return (-1);
		free(env->g_env[idx]);
		env->g_env[idx] = entry;
		return (0);
	}
	if (!entry)
		return (-1);
	return (add_entry(env, entry));
}
