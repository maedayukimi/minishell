/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 19:38:22 by mawako            #+#    #+#             */
/*   Updated: 2025/05/21 15:42:45 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "env_management.h"

int	g_sig_subshell = 0;

void	fatal_error(const char *msg)
{
	fprintf(stderr, "Fatal error: %s\n", msg);
	exit(1);
}

int	exec_sh_c(char **argv, t_env *env)
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
	expansion(node, env);
	setup_heredoc(node, env);
	status = exec(node, env);
	free_node(node);
	free_token(words);
	return (status);
}

// int	check_quote(const char *line)
// {
// 	int	i;
// 	int	sq;
// 	int	dq;

// 	i = 0;
// 	sq = 0;
// 	dq = 0;
// 	while (line[i])
// 	{
// 		if (line[i] == '\'' && !dq)
// 			sq = !sq;
// 		else if (line[i] == '"' && !sq)
// 			dq = !dq;
// 		i++;
// 	}
// 	return (!sq && !dq);
// }

void	set_init(char **envp, t_env *env)
{
	struct sigaction	sa;

	init_env(envp, env);
	env->environ = NULL;
	env->last_bg_pid = 0;
	env->last_exit_status = 0;
	env->environ = env->g_env;
	signal(SIGINT, sigint_handler);
	signal(SIGQUIT, SIG_IGN);
	sa.sa_handler = sigchld_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sigaction(SIGCHLD, &sa, NULL);
}

int	main(int argc, char **argv, char **envp)
{
	int			status;
	char		*line;
	t_env		env;

	(void)argv;
	(void)argc;
	status = 0;
	set_init(envp, &env);
	while (1)
	{
		line = get_complete_input();
		if (!line)
			break ;
		if (*line)
			add_history(line);
		interpret(line, &status, &env);
		free(line);
	}
	free_env(&env);
	return (status);
}
