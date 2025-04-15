/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 19:38:22 by mawako            #+#    #+#             */
/*   Updated: 2025/04/15 19:43:17 by mawako           ###   ########.fr       */
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

int	exec_sh_c(char **argv)
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

int	check_quote(const char *line)
{
	int	i;
	int	sq;
	int	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (line[i])
	{
		if (line[i] == '\'' && !dq)
			sq = !sq;
		else if (line[i] == '"' && !sq)
			dq = !dq;
		i++;
	}
	return (!sq && !dq);
}

char	*get_complete_input(void)
{
	char	*line;
	char	*tmp;
	char	*buffer;

	line = readline("minishell$ ");
	if (!line)
		return (NULL);
	while (!check_quote(line))
	{
		tmp = readline("> ");
		if (!tmp)
			break ;
		buffer = ft_strjoin(line, "\n");
		free(line);
		line = ft_strjoin(buffer, tmp);
		free(buffer);
		free(tmp);
	}
	return (line);
}

int	main(int argc, char **argv, char **envp)
{
	int			status;
	char		*line;
	struct sigaction	sa;

	(void)argv;
	(void)argc;
	init_env(envp);
	environ = g_env;
	status = 0;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &sa, NULL);
	while (1)
	{
		line = get_complete_input();
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
