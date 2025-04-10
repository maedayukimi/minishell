/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipeline_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 16:04:53 by mawako            #+#    #+#             */
/*   Updated: 2025/04/10 16:13:57 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_node *node)
{
	char	**argv;
	char	*cmd_path;

	argv = create_argv(node->args);
	if (!argv || !argv[0])
		exit(0);
	if (is_builtin(argv[0]))
		exit(exec_builtin(argv));
	else
	{
		cmd_path = search_path(argv[0]);
		if (!cmd_path)
			fatal_error("command not found");
		execve(cmd_path, argv, environ);
		fatal_error("execve failed");
	}
}

void	setup_pipe_child(t_node *node, int i, int n, int **pipes)
{
	setup_dup(i, n, pipes);
	close_all_pipes(n, pipes);
	if (node->redirects)
	{
		open_redir_file(node->redirects);
		do_redirect(node->redirects);
	}
	execute_command(node);
}

pid_t	*setup_pipe_children(t_node *head, int n, int **pipes)
{
	pid_t	*pids;
	t_node	*cur;
	int		i;

	i = 0;
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		fatal_error("malloc failed");
	cur = head;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			fatal_error("fork failed");
		if (pids[i] == 0)
			setup_pipe_child(cur, i, n, pipes);
		cur = cur->next;
		i++;
	}
	return (pids);
}

int	wait_pipeline_children(pid_t *pids, int n)
{
	int	i;
	int	status;
	int	sigint_error;

	i = 0;
	sigint_error = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
			sigint_error = 1;
		i++;
	}
	if (sigint_error)
		write(1, "\n", 1);
	return (status);
}

int	exec_pipeline(t_node *head)
{
	int		n;
	int		status;
	pid_t	*pids;
	int		**pipes;

	n = count_pipeline_nodes(head);
	pipes = setup_pipes(n);
	pids = setup_pipe_children(head, n, pipes);
	cleanup_pipes(pipes, n);
	signal(SIGINT, SIG_IGN);
	status = wait_pipeline_children(pids, n);
	signal(SIGINT, sigint_handler);
	free(pids);
	return (status);
}
