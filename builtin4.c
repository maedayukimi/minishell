/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:59:44 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 22:04:06 by mawako           ###   ########.fr       */
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
		exit_status = ft_atoi(argv[1]);
	exit(exit_status);
	return (exit_status);
}
