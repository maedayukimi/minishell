/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 19:38:22 by mawako            #+#    #+#             */
/*   Updated: 2025/04/03 15:30:13 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env_management.h"

char	**environ = NULL;
pid_t	g_last_bg_pid = 0;
int		g_last_exit_status = 0;

void	fatal_error(const char *msg)
{
	fprintf(stderr, "Fatal error: %s\n", msg);
	exit(1);
}

void	free_strs(char **strs)
{
	int	i;

	if (!strs)
		return ;
	i = 0;
	while (strs[i])
	{
		free(strs[i]);
		i++;
	}
	free(strs);
}

char	*search_path(const char *filename)
{
	char	**paths;
	char	*path_env;
	char	*full_path;
	int		i;

	if (!filename || strchr(filename, '/'))
		return (strdup(filename));
	path_env = getenv("PATH");
	if (!path_env)
		return (NULL);
	paths = ft_split(path_env, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = malloc(strlen(paths[i]) + strlen(filename) + 2);
		if (!full_path)
			break ;
		sprintf(full_path, "%s/%s", paths[i], filename);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	free_strs(paths);
	return (NULL);
}

static int	exec_sh_c(char **argv)
{
	char	*subcmd;
	int		status;
	t_token	*words;
	t_node	*node;

	subcmd = argv[2];
	status = 0;
	words = tokenize(subcmd);
	if (!words || words->kind == TK_EOF)
		return (0);
	node = parse(words);
	if (!node)
	{
		free_token(words);
		return (258);
	}
	expansion(node);
	setup_heredoc(node);
	status = exec(node);
	free_node(node);
	free_token(words);
	return (status);
}

int	exec_pipeline(t_node *head)
{
	int		n;
	int		i;
	int		j;
	int		status;
	int		(*pipes)[2];
	pid_t	*pids;
	t_node	*cur;
	char	**argv;
	char	*cmd_path;

	n = 0;
	pipes = NULL;
	cur = head;
	while (cur)
	{
		n++;
		cur = cur->next;
	}
	if (n > 1)
	{
		pipes = malloc(sizeof(int[2]) * (n - 1));
		if (!pipes)
			fatal_error("malloc failed");
		i = 0;
		while (i < n - 1)
		{
			if (pipe(pipes[i]) == -1)
				fatal_error("pipe failed");
			fprintf(stderr, "[DEBUG] Created pipe %d: read_fd=%d, write_fd=%d\n",
				i, pipes[i][0], pipes[i][1]);
			i++;
		}
	}
	pids = malloc(sizeof(pid_t) * n);
	if (!pids)
		fatal_error("malloc failed");
	cur = head;
	i = 0;
	while (i < n)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			fatal_error("fork failed");
		if (pids[i] == 0)
		{
			if (i > 0)
			{
				if (dup2(pipes[i - 1][0], STDIN_FILENO) == -1)
					fatal_error("dup2 failed");
			}
			if (i < n - 1)
			{
				if (dup2(pipes[i][1], STDOUT_FILENO) == -1)
					fatal_error("dup2 failed");
			}
			if (pipes)
			{
				j = 0;
				while (j < n - 1)
				{
					close(pipes[j][0]);
					close(pipes[j][1]);
					j++;
				}
			}
			if (cur->redirects)
			{
				open_redir_file(cur->redirects);
				do_redirect(cur->redirects);
			}
			{
				argv = create_argv(cur->args);
				if (!argv || !argv[0])
					exit(0);
				fprintf(stderr, "[DEBUG][Child %d] Executing command: %s\n", getpid(), argv[0]);
				if (is_builtin(argv[0]))
				{
					status = exec_builtin(argv);
					free(argv);
					exit(status);
				}
				else
				{
					cmd_path = search_path(argv[0]);
					if (!cmd_path)
						fatal_error("command not found");
					execve(cmd_path, argv, environ);
					fatal_error("execve failed");
				}
			}
		}
		cur = cur->next;
		i++;
	}
	if (pipes)
	{
		i = 0;
		while (i < n - 1)
		{
			close(pipes[i][0]);
			close(pipes[i][1]);
			i++;
		}
		free(pipes);
	}
	i = 0;
	while (i < n)
	{
		waitpid(pids[i], &status, 0);
		fprintf(stderr, "[DEBUG][Parent] Child PID %d exited with status %d\n", pids[i], status);
		i++;
	}
	free(pids);
	return (status);
}

int	exec_cmd(t_node *node)
{
	char	**argv;
	char	*path;
	int		status;
	int		wstatus;
	pid_t	pid;

	argv = create_argv(node->args);
	if (!argv || !argv[0])
	{
		free(argv);
		return (0);
	}
	if ((!strcmp(argv[0], "sh") || !strcmp(argv[0], "bash")
			|| !strcmp(argv[0], "zsh"))
		&& argv[1] && !strcmp(argv[1], "-c") && argv[2])
	{
		status = exec_sh_c(argv);
		free(argv);
		return (status);
	}
	if (is_builtin(argv[0]) && node->redirects == NULL)
	{
		status = exec_builtin(argv);
		free(argv);
		return (status);
	}
	pid = fork();
	if (pid < 0)
		fatal_error("fork failed");
	if (pid == 0)
	{
		open_redir_file(node->redirects);
		do_redirect(node->redirects);
		if (is_builtin(argv[0]))
		{
			status = exec_builtin(argv);
			exit(status);
		}
		else
		{
			path = search_path(argv[0]);
			if (!path)
				fatal_error("command not found");
			execve(path, argv, environ);
			fatal_error("execve failed");
		}
	}
	else
	{
		waitpid(pid, &wstatus, 0);
		free(argv);
		if (WIFEXITED(wstatus))
			return (WEXITSTATUS(wstatus));
		else if (WIFSIGNALED(wstatus))
			return (128 + WTERMSIG(wstatus));
		return (1);
	}
}

int	exec(t_node *node)
{
	int		status;
	t_node	*pipeline_head;

	status = 0;
	while (node)
	{
		if (node->separator && strcmp(node->separator, "|") == 0)
		{
			pipeline_head = node;
			while (node && node->separator && strcmp(node->separator, "|") == 0)
				node = node->next;
			status = exec_pipeline(pipeline_head);
		}
		else
		{
			status = exec_cmd(node);
			if (node->separator)
			{
				if (!strcmp(node->separator, "&&"))
				{
					if (status != 0)
						break ;
				}
				else if (!strcmp(node->separator, "||"))
				{
					if (status == 0)
						break ;
				}
			}
			node = node->next;
		}
	}
	return (status);
}

void	interpret(char *line, int *stat_loc)
{
	t_token	*words;
	t_node	*node;

	words = tokenize(line);
	if (!words || words->kind == TK_EOF)
	{
		*stat_loc = ERROR_TOKENIZE;
		return ;
	}
	node = parse(words);
	if (!node)
	{
		*stat_loc = 258;
		return ;
	}
	expansion(node);
	setup_heredoc(node);
	*stat_loc = exec(node);
	free_node(node);
}

int	main(int argc, char **argv, char **envp)
{
	int		status;
	char	*line;

	(void)argv;
	(void)argc;
	init_env(envp);
	environ = g_env;
	status = 0;
	while (1)
	{
		line = readline("minishell$ ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		interpret(line, &status);
		free(line);
	}
	free_env();
	return (status);
}
