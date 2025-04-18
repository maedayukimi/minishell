/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:10:12 by mawako            #+#    #+#             */
/*   Updated: 2025/04/09 19:10:28 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(const char *name)
{
	extern char	**environ;
	int			i;
	int			len;

	len = strlen(name);
	i = 0;
	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
		i++;
	}
	return ("");
}

char	*get_shell_pid_str(void)
{
	int		fd;
	char	buf[256];
	int		n;
	int		i;
	char	*pid_str;

	fd = open("/proc/self/stat", O_RDONLY);
	if (fd < 0)
		return (strdup("0"));
	n = read(fd, buf, 255);
	if (n <= 0)
	{
		close(fd);
		return (strdup("0"));
	}
	buf[n] = '\0';
	close(fd);
	i = 0;
	while (buf[i] && buf[i] != ' ')
		i++;
	buf[i] = '\0';
	pid_str = strdup(buf);
	return (pid_str);
}
