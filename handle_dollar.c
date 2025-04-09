/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_dollar.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:14:31 by mawako            #+#    #+#             */
/*   Updated: 2025/04/09 19:15:11 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_pattern1(char **result, int *i)
{
	char	*status;

	status = expand_exit_status();
	*result = ft_strjoin_and_free(*result, status);
	*i += 2;
}

static void	expand_pattern2(char **result, int *i)
{
	char	*bg_pid_str;

	bg_pid_str = ft_itoa(g_last_bg_pid);
	*result = ft_strjoin_and_free(*result, bg_pid_str);
	*i += 2;
}

static void	expand_pattern3(char **result, int *i)
{
	char	*pid_str;

	pid_str = get_shell_pid_str();
	*result = ft_strjoin_and_free(*result, pid_str);
	*i += 2;
}

static void	expand_pattern4(char **result, const char *str, int *i)
{
	int		start;
	char	*key;
	char	*val;

	start = ++(*i);
	while (is_var_char(str[*i]))
		(*i)++;
	key = ft_substr(str, (unsigned int)start, *i - start);
	val = get_env_value(key);
	*result = ft_strjoin_and_free(*result, strdup(val));
	free(key);
}

void	handle_dollar(const char *str, int *i, char **result)
{
	if (str[*i + 1] == '?')
		expand_pattern1(result, i);
	else if (str[*i + 1] == '!')
		expand_pattern2(result, i);
	else if (str[*i + 1] == '$')
		expand_pattern3(result, i);
	else if (is_var_char(str[*i + 1]))
		expand_pattern4(result, str, i);
	else
		append_char(result, str[(*i)++]);
}
