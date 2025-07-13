/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:16 by mawako            #+#    #+#             */
/*   Updated: 2025/07/13 14:39:48 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)//ft_strcmp
{
	if (!cmd)
		return (0);
	if (!ft_strcmp(cmd, "echo"))
		return (1);
	if (!ft_strcmp(cmd, "cd"))
		return (1);
	if (!ft_strcmp(cmd, "pwd"))
		return (1);
	if (!ft_strcmp(cmd, "env"))
		return (1);
	if (!ft_strcmp(cmd, "export"))
		return (1);
	if (!ft_strcmp(cmd, "unset"))
		return (1);
	if (!ft_strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(char **argv, t_env *env)//ft_strcmp
{
	if (!argv[0])
		return (1);
	if (!ft_strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	else if (!ft_strcmp(argv[0], "cd"))
		return (builtin_cd(argv));
	else if (!ft_strcmp(argv[0], "pwd"))
		return (builtin_pwd());
	else if (!ft_strcmp(argv[0], "env"))
		return (builtin_env(argv, env));
	else if (!ft_strcmp(argv[0], "export"))
		return (builtin_export(argv, env));
	else if (!ft_strcmp(argv[0], "unset"))
		return (builtin_unset(argv, env));
	else if (!ft_strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}
