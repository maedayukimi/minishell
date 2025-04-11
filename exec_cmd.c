/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:33:55 by mawako            #+#    #+#             */
/*   Updated: 2025/04/11 14:01:56 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	exec_child(t_node *node, char **argv)
{
	char	*path;

	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	if (node->redirects)
	{
		open_redir_file(node->redirects);
		do_redirect(node->redirects);
	}
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

static int	exec_parent(char **argv, pid_t pid)
{
	int	wstatus;

	signal(SIGINT, SIG_IGN);
	waitpid(pid, &wstatus, 0);
	if (WIFSIGNALED(wstatus) && WTERMSIG(wstatus) == SIGINT)
		write(1, "\n", 1);
	signal(SIGINT, sigint_handler);
	free_argv(argv);
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	else if (WIFSIGNALED(wstatus))
		return (128 + WTERMSIG(wstatus));
	return (1);
}

static int	handle_sh_builtin(t_node *node, char **argv, int *handle)
{
	int	status;

	*handle = 0;
	if ((!strcmp(argv[0], "sh") || !strcmp(argv[0], "bash")
			|| !strcmp(argv[0], "zsh")) && argv[1]
		&& !strcmp(argv[1], "-c") && argv[2])
	{
		status = exec_sh_c(argv);
		free_argv(argv);
		*handle = 1;
		return (status);
	}
	if (is_builtin(argv[0]) && node->redirects == NULL)
	{
		status = exec_builtin(argv);
		free_argv(argv);
		*handle = 1;
		return (status);
	}
	return (0);
}

int	exec_cmd(t_node *node)
{
	char	**argv;
	int		status;
	pid_t	pid;
	int		handle;

	argv = create_argv(node->args);
	if (!argv || !argv[0])
	{
		free_argv(argv);
		return (0);
	}
	status = handle_sh_builtin(node, argv, &handle);
	if (handle)
		return (status);
	pid = fork();
	if (pid < 0)
		fatal_error("fork failed");
	if (pid == 0)
		exec_child(node, argv);
	else
		return (exec_parent(argv, pid));
	return (0);
}
