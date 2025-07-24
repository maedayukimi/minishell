/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   search_path.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:41:51 by shuu              #+#    #+#             */
/*   Updated: 2025/07/24 13:18:13 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "minishell.h"

static char	*check_full_path(char **paths, const char *filename)
{
	char	*full_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		full_path = malloc(ft_strlen(paths[i]) + ft_strlen(filename) + 2);
		if (!full_path)
			break ;
		ft_sprintf(full_path, "%s/%s", paths[i], filename);
		if (access(full_path, X_OK) == 0)
		{
			free_strs(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_strs(paths);
	return (NULL);
}

char	*search_path(const char *filename)
{
	char	**paths;
	char	*path_env;

	if (!filename || *filename == '\0')
		return (NULL);
	if (ft_strchr(filename, '/'))
		return (ft_strdup(filename));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	return (check_full_path(paths, filename));
}
