/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 20:19:30 by mawako            #+#    #+#             */
/*   Updated: 2025/07/13 15:41:34 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"
#include "minishell.h"

void	init_env(char **envp, t_env *env)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	env->g_env = malloc((count + 1) * sizeof(char *));
	if (!env->g_env)
	{
		perror("malloc");
		exit(1);
	}
	while (i < count)
	{
		env->g_env[i] = ft_strdup(envp[i]);
		if (!env->g_env[i++])
		{
			perror("ft_strdup");
			exit(1);
		}
	}
	env->g_env[count] = (NULL);
}

char	*my_getenv(const char *name, t_env *env)
{
	int		i;
	size_t	len;

	i = 0;
	if (!name || !env->g_env)
		return (NULL);
	len = ft_strlen(name);
	while (env->g_env[i])
	{
		if (ft_strncmp(env->g_env[i], name, len) == 0 && env->g_env[i][len] == '=')
			return (env->g_env[i] + len + 1);
		i++;
	}
	return (NULL);
}

void	free_env(t_env *env)
{
	int	i;

	i = 0;
	if (!env->g_env)
		return ;
	while (env->g_env[i])
		free(env->g_env[i++]);
	free(env->g_env);
	env->g_env = (NULL);
}

int	my_unsetenv(const char *name, t_env *env)
{
	int		i;
	int		j;
	size_t	len;

	i = 0;
	j = 0;
	if (!name || !env->g_env)
		return (-1);
	len = ft_strlen(name);
	while (env->g_env[i])
	{
		if (ft_strncmp(env->g_env[i], name, len) == 0 && env->g_env[i][len] == '=')
		{
			free(env->g_env[i]);
			while (env->g_env[j])
			{
				env->g_env[j] = env->g_env[j + 1];
				j++;
			}
			i--;
		}
	}
	return (0);
}
