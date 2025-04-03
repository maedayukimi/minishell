/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:22:06 by mawako            #+#    #+#             */
/*   Updated: 2025/04/03 15:32:23 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

char	**g_env = NULL;

void	init_env(char **envp)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	g_env = malloc((count + 1) * sizeof(char *));
	if (!g_env)
	{
		perror("malloc");
		exit(1);
	}
	while (i < count)
	{
		g_env[i] = strdup(envp[i]);
		if (!g_env[i++])
		{
			perror("strdup");
			exit(1);
		}
	}
	g_env[count] = (NULL);
}

char	*my_getenv(const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	if (!name || !g_env)
		return (NULL);
	len = strlen(name);
	while (g_env[i])
	{
		if (strncmp(g_env[i], name, len) == 0 && g_env[i][len] == '=')
			return (g_env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	my_setenv(const char *name, const char *value, int overwrite)
{
	int		i;
	size_t	len;
	char	*new_entry;
	int		count;
	char	**new_env;

	i = 0;
	count = 0;
	if (!name || strchr(name, '='))
		return (-1);
	len = strlen(name);
	if (g_env)
	{
		while (g_env[i])
		{
			count++;
			if (strncmp(g_env[i], name, len) == 0 && g_env[i][len] == '=')
			{
				if (!overwrite)
					return (0);
				new_entry = malloc(len + strlen(value) + 2);
				if (!new_entry)
					return (-1);
				sprintf(new_entry, "%s=%s", name, value);
				free(g_env[i]);
				g_env[i] = new_entry;
				return (0);
			}
			i++;
		}
	}
	{
		new_env = realloc(g_env, (count + 2) * sizeof(char *));
		if (!new_env)
			return (-1);
		g_env = new_env;
		new_entry = malloc(len + strlen(value) + 2);
		if (!new_entry)
			return (-1);
		sprintf(new_entry, "%s=%s", name, value);
		g_env[count] = new_entry;
		g_env[count + 1] = NULL;
		return (0);
	}
}

int	my_unsetenv(const char *name)
{
	int		i;
	int		j;
	size_t	len;

	i = 0;
	j = 0;
	if (!name || !g_env)
		return (-1);
	len = strlen(name);
	while (g_env[i])
	{
		if (strncmp(g_env[i], name, len) == 0 && g_env[i][len] == '=')
		{
			free(g_env[i]);
			while (g_env[j])
			{
				g_env[j] = g_env[j + 1];
				j++;
			}
			i--;
		}
	}
	return (0);
}

void	free_env(void)
{
	int	i;

	i = 0;
	if (!g_env)
		return ;
	while (g_env[i])
		free(g_env[i++]);
	free(g_env);
	g_env = (NULL);
}
