/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin6.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 22:02:57 by mawako            #+#    #+#             */
/*   Updated: 2025/07/13 15:16:31 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	print_env(char **e)
{
	int	j;

	j = 0;
	while (e && e[j])
		printf("declare -x %s\n", e[j++]);
}

static void	process_arg(char *kv, t_env *env)
{
	char	*eq;
	char	*val;

	eq = ft_strchr(kv, '=');
	if (!eq)
	{
		val = getenv(kv);
		if (!val)
			val = "";
		update_env(kv, val, env);
	}
	else
	{
		*eq = '\0';
		update_env(kv, eq + 1, env);
		*eq = '=';
	}
}

int	builtin_export(char **argv, t_env *env)
{
	int	i;

	if (!argv[1])
	{
		print_env(env->g_env);
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		process_arg(argv[i], env);
		i++;
	}
	return (0);
}
