/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 19:38:22 by mawako            #+#    #+#             */
/*   Updated: 2025/05/06 17:15:43 by mawako           ###   ########.fr       */
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

static int	line_ends_with_pipe(const char *line)
{
	size_t	i;

	i = strlen(line);
	while (i > 0 && (line[i - 1] == '\n'
			|| isspace((unsigned char)line[i - 1])))
		i--;
	if (i > 0 && line[i - 1] == '|')
		return (1);
	return (0);
}

static int	ends_with_bs(const char *line, size_t *count)
{
	size_t	i;
	size_t	c;

	i = strlen(line);
	while (i > 0 && (line[i - 1] == '\n'
			|| isspace((unsigned char)line[i - 1])))
		i--;
	c = 0;
	while (i > 0 && line[i - 1] == '\\')
	{
		c++;
		i--;
	}
	*count = c;
	if (c % 2 == 1)
		return (1);
	return (0);
}

static char	*join_bs_line(char *line, char *more, size_t bs_count)
{
	char	*new;
	size_t	prefix;
	size_t	i;

	prefix = strlen(line);
	while (prefix > 0 && (line[prefix - 1] == '\n'
			|| isspace((unsigned char)line[prefix - 1])))
		prefix--;
	prefix -= bs_count;
	new = malloc(prefix + bs_count - 1 + strlen(more) + 1);
	if (new == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < bs_count - 1)
	{
		new[prefix + i] = '\\';
		i++;
	}
	memcpy(new, line, prefix);
	memcpy(new + prefix + i, more, strlen(more));
	new[prefix + i + strlen(more)] = '\0';
	free(line);
	free(more);
	return (new);
}

static char	*join_pipe_line(char *line, char *more)
{
	char	*new;
	size_t	len;
	size_t	i;

	len = strlen(line);
	new = malloc(len + 1 + strlen(more) + 1);
	if (new == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < len)
	{
		new[i] = line[i];
		i++;
	}
	new[i++] = '\n';
	memcpy(new + i, more, strlen(more));
	new[i + strlen(more)] = '\0';
	free(line);
	free(more);
	return (new);
}

char	*get_complete_input(void)
{
	char	*line;
	char	*more;
	size_t	bs_count;

	line = readline("minishell$ ");
	if (line == NULL)
		return (NULL);
	while (line_ends_with_pipe(line)
		|| ends_with_bs(line, &bs_count))
	{
		more = readline("> ");
		if (more == NULL)
			break ;
		if (ends_with_bs(line, &bs_count))
			line = join_bs_line(line, more, bs_count);
		else
			line = join_pipe_line(line, more);
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
