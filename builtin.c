/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:03:45 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 16:00:09 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	env_size(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

int	env_index(char **envp, const char *key)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(key);
	while (envp && envp[i])
	{
		if (strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

char	*make_env_entry(const char *key, const char *value)
{
	char	*e;
	int		len;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	e = malloc(len);
	if (!e)
		fatal_error("malloc failed in make_env_entry");
	ft_sprintf(e, "%s=%s", key, value);
	return (e);
}

char	**extend_env_array(char **envp, char *entry)
{
	char	**ne;
	int		n;
	int		i;

	n = env_size(envp);
	ne = malloc(sizeof(char *) * (n + 2));
	if (!ne)
		fatal_error("malloc failed in extend_env_array");
	i = 0;
	while (i < n)
	{
		ne[i] = envp[i];
		i++;
	}
	ne[i] = entry;
	i++;
	ne[i] = NULL;
	return (ne);
}

char	**delete_env_array(char **envp, int idx)
{
	char	**ne;
	int		n;
	int		i;
	int		j;

	n = env_size(envp);
	ne = malloc(sizeof(char *) * n);
	if (!ne)
		fatal_error("malloc failed in delete_env_array");
	i = 0;
	j = 0;
	while (i < n)
	{
		if (i != idx)
		{
			ne[j] = envp[i];
			j++;
		}
		i++;
	}
	ne[j] = NULL;
	return (ne);
}
