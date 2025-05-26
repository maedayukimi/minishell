/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:59:44 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 16:01:45 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	builtin_env(char **argv, t_env *env)
{
	int	i;

	i = 0;
	(void)argv;
	while (env->environ[i])
	{
		printf("%s\n", env->environ[i]);
		i++;
	}
	return (0);
}

int	builtin_export(char **argv, t_env *env)
{
	int		i;
	int		j;
	char	*key_val;
	char	*eq;
	char	*val;

	if (!argv[1])
	{
		j = 0;
		while (env->g_env && env->g_env[j])
		{
			printf("declare -x %s\n", env->g_env[j]);
			j++;
		}
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		key_val = argv[i];
		eq = strchr(key_val, '=');
		if (!eq)
		{
			val = getenv(key_val);
			if (!val)
				val = "";
			update_env(key_val, val, env);
		}
		else
		{
			*eq = '\0';
			update_env(key_val, eq + 1, env);
			*eq = '=';
		}
		i++;
	}
	return (0);
}

int	builtin_unset(char **argv, t_env *env)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		remove_env(argv[i], env);
		unsetenv(argv[i]);
		i++;
	}
	env->environ = env->g_env;
	return (0);
}

int	builtin_exit(char **argv)
{
	int	exit_status;

	exit_status = 0;
	fprintf(stderr, "exit\n");
	if (argv[1])
		exit_status = atoi(argv[1]);
	exit(exit_status);
	return (exit_status);
}
