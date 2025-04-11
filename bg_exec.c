/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bg_exec.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:28:05 by mawako            #+#    #+#             */
/*   Updated: 2025/04/11 15:06:11 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	setup_child_bg(t_node *node)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (node->redirects)
	{
		open_redir_file(node->redirects);
		do_redirect(node->redirects);
	}
}

static void	child_execute_bg(char **argv)
{
	char	*path;

	if (is_builtin(argv[0]))
		exit(exec_builtin(argv));
	else
	{
		path = search_path(argv[0]);
		if (!path)
			fatal_error("command not found");
		execve(path, argv, environ);
		fatal_error("execve failed");
	}
}

int	exec_background(t_node *node)
{
	char	**argv;
	pid_t	pid;

	argv = create_argv(node->args);
	if (!argv || !argv[0])
	{
		free_argv(argv);
		return (0);
	}
	pid = fork();
	if (pid < 0)
		fatal_error("fork failed");
	if (pid == 0)
	{
		setup_child_bg(node);
		child_execute_bg(argv);
	}
	else
	{
		g_last_bg_pid = pid;
		printf("[%d] %d\n", 1, pid);
		free_argv(argv);
		return (0);
	}
	return (0);
}
