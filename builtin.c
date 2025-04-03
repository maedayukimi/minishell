/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:03:45 by mawako            #+#    #+#             */
/*   Updated: 2025/04/01 16:58:24 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern char	**environ;

static int	is_valid_echo_option(const char *str)
{
	int	i;

	if (!str || str[0] != '-' || !str[1])
		return (0);
	i = 1;
	while (str[i])
	{
		if (str[i] != 'n' && str[i] != 'e' && str[i] != 'E')
			return (0);
		i++;
	}
	return (1);
}

static char	*interpret_escapes(const char *str)
{
	size_t	len;
	char	*buf;
	size_t	i;
	size_t	j;

	len = strlen(str);
	buf = malloc(len * 2 + 1);
	i = 0;
	j = 0;
	if (!buf)
		return (NULL);
	while (str[i])
	{
		if (str[i] == '\\')
		{
			i++;
			if (str[i] == 'n')
				buf[j++] = '\n';
			else if (str[i] == 't')
				buf[j++] = '\t';
			else if (str[i] == 'r')
				buf[j++] = '\r';
			else if (str[i] == '\\')
				buf[j++] = '\\';
			else
			{
				buf[j++] = '\\';
				if (str[i])
					buf[j++] = str[i];
			}
			if (str[i])
				i++;
		}
		else
			buf[j++] = str[i++];
	}
	buf[j] = '\0';
	return (buf);
}

static int	builtin_echo(char **argv)
{
	int		i;
	int		j;
	int		newline;
	int		interpret;
	char	*processed;

	i = 1;
	newline = 1;
	interpret = 0;
	if (argv[1] && is_valid_echo_option(argv[1]))
	{
		j = 1;
		while (argv[1][j])
		{
			if (argv[1][j] == 'n')
				newline = 0;
			else if (argv[1][j] == 'e')
				interpret = 1;
			j++;
		}
		i++;
	}
	while (argv[i])
	{
		if (interpret)
		{
			processed = interpret_escapes(argv[i]);
			if (processed)
			{
				printf("%s", processed);
				free(processed);
			}
		}
		else
			printf("%s", argv[i]);
		if (argv[i + 1])
			printf(" ");
		i++;
	}
	if (newline)
		printf("\n");
	return (0);
}

static int	builtin_cd(char **argv)
{
	char	*home;

	if (!argv[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		if (chdir(home) != 0)
		{
			perror("cd");
			return (1);
		}
		return (0);
	}
	if (chdir(argv[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

static int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

static int	builtin_env(char **argv)
{
	int	i;

	i = 0;
	(void)argv;
	while (environ[i])
	{
		printf("%s\n", environ[i]);
		i++;
	}
	return (0);
}

static int	builtin_export(char **argv)
{
	int		i;
	int		j;
	char	*key_val;
	char	*eq;

	if (!argv[1])
	{
		j = 0;
		while (environ[j])
		{
			printf("declare -x %s\n", environ[j]);
			j++;
		}
		return (0);
	}
	i = 1;
	while (argv[i])
	{
		key_val = argv[i];
		eq = strchr(key_val, '=');
		if (!eq)
		{
			fprintf(stderr, "export: invalid format: %s\n", key_val);
		}
		else
		{
			*eq = '\0';
			setenv(key_val, eq + 1, 1);
			*eq = '=';
		}
		i++;
	}
	return (0);
}

static int	builtin_unset(char **argv)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		unsetenv(argv[i]);
		i++;
	}
	return (0);
}

static int	builtin_exit(char **argv)
{
	int	exit_status;

	exit_status = 0;
	fprintf(stderr, "exit\n");
	if (argv[1])
		exit_status = atoi(argv[1]);
	exit(exit_status);
	return (exit_status);
}

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

int	exec_builtin(char **argv)
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
		return (builtin_env(argv));
	else if (!strcmp(argv[0], "export"))
		return (builtin_export(argv));
	else if (!strcmp(argv[0], "unset"))
		return (builtin_unset(argv));
	else if (!strcmp(argv[0], "exit"))
		return (builtin_exit(argv));
	return (1);
}
