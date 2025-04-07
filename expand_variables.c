/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:14:12 by mawako            #+#    #+#             */
/*   Updated: 2025/04/07 18:03:46 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_char_and_free(char *s, char c)
{
	int		len;
	char	*res;

	len = strlen(s);
	res = malloc(len + 2);
	if (!res)
		return (NULL);
	memcpy(res, s, len);
	res[len] = c;
	res[len + 1] = '\0';
	free(s);
	return (res);
}

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

int	is_var_char(char c)
{
	return (isalnum(c) || c == '_');
}

static char	*get_env_value(const char *name)
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

static char	*expand_exit_status(void)
{
	extern int	g_last_exit_status;

	return (ft_itoa(g_last_exit_status));
}

static char	*get_shell_pid_str(void)
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

char	*expand_variables(char *str)
{
	char	*result;
	int		i;
	char	*status;
	int		start;
	char	*key;
	char	*val;
	char	*pid_str;

	result = calloc(1, 1);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				status = expand_exit_status();
				result = ft_strjoin_and_free(result, status);
				i += 2;
			}
			else if (str[i + 1] == '!')
			{
				pid_str = ft_itoa(g_last_bg_pid);
				result = ft_strjoin_and_free(result, pid_str);
				i += 2;
			}
			else if (str[i + 1] == '$')
			{
				pid_str = get_shell_pid_str();
				result = ft_strjoin_and_free(result, pid_str);
				i += 2;
			}
			else if (is_var_char(str[i + 1]))
			{
				start = ++i;
				while (is_var_char(str[i]))
					i++;
				key = ft_substr(str, start, i - start);
				val = get_env_value(key);
				result = ft_strjoin_and_free(result, strdup(val));
				free(key);
			}
			else
			{
				result = ft_strjoin_char_and_free(result, str[i]);
				i++;
			}
		}
		else
		{
			result = ft_strjoin_char_and_free(result, str[i]);
			i++;
		}
	}
	return (result);
}
