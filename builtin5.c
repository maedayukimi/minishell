/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin5.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 16:00:16 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 16:00:27 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (!strcmp(cmd, "echo"))
		return (1);
	if (!strcmp(cmd, "cd"))
		return (1);
	if (!strcmp(cmd, "pwd"))
		return (1);
	if (!strcmp(cmd, "env"))
		return (1);
	if (!strcmp(cmd, "export"))
		return (1);
	if (!strcmp(cmd, "unset"))
		return (1);
	if (!strcmp(cmd, "exit"))
		return (1);
	return (0);
}

int	exec_builtin(char **argv, t_env *env)
{
	if (!argv[0])
		return (1);
	if (!strcmp(argv[0], "echo"))
		return (builtin_echo(argv));
	else if (!strcmp(argv[0], "cd"))
		return (builtin_cd(argv));
	else if (!strcmp(argv[0], "pwd"))
		return (builtin_pwd());
	else if (!strcmp(argv[0], "env"))
		return (builtin_env(argv, env));
	else if (!strcmp(argv[0], "export"))
		return (builtin_export(argv, env));
	else if (!strcmp(argv[0], "unset"))
		return (builtin_unset(argv, env));
	else if (!strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}
