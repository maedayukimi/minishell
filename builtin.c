/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 18:03:45 by mawako            #+#    #+#             */
/*   Updated: 2025/05/23 20:12:13 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	env_size(char **envp)
{
	int	i;

	i = 0;
	while (envp && envp[i])
		i++;
	return (i);
}

static int	env_index(char **envp, const char *key)
{
	int	i;
	int	len;

	i = 0;
	len = ft_strlen(key);
	while (envp && envp[i])
	{
		if (strncmp(envp[i], key, len) == 0 && envp[i][len] == '=')
			return (i);
		i++;
	}
	return (-1);
}

static char	*make_env_entry(const char *key, const char *value)
{
	char	*e;
	int		len;

	len = ft_strlen(key) + ft_strlen(value) + 2;
	e = malloc(len);
	if (!e)
		fatal_error("malloc failed in make_env_entry");
	ft_sprintf(e, "%s=%s", key, value);
	return (e);
}

static char	**extend_env_array(char **envp, char *entry)
{
	char	**ne;
	int		n;
	int		i;

	n = env_size(envp);
	ne = malloc(sizeof(char *) * (n + 2));
	if (!ne)
		fatal_error("malloc failed in extend_env_array");
	i = 0;
	while (i < n)
	{
		ne[i] = envp[i];
		i++;
	}
	ne[i] = entry;
	i++;
	ne[i] = NULL;
	return (ne);
}

static char	**delete_env_array(char **envp, int idx)
{
	char	**ne;
	int		n;
	int		i;
	int		j;

	n = env_size(envp);
	ne = malloc(sizeof(char *) * n);
	if (!ne)
		fatal_error("malloc failed in delete_env_array");
	i = 0;
	j = 0;
	while (i < n)
	{
		if (i != idx)
		{
			ne[j] = envp[i];
			j++;
		}
		i++;
	}
	ne[j] = NULL;
	return (ne);
}

void	update_env(const char *k, const char *v, t_env *e)
{
	int		idx;
	char	*ent;
	char	**ne;

	idx = env_index(e->g_env, k);
	ent = make_env_entry(k, v);
	if (idx >= 0)
	{
		free(e->g_env[idx]);
		e->g_env[idx] = ent;
	}
	else
	{
		ne = extend_env_array(e->g_env, ent);
		free(e->g_env);
		e->g_env = ne;
	}
	e->environ = e->g_env;
}

void	remove_env(const char *k, t_env *e)
{
	int		idx;
	char	**ne;

	idx = env_index(e->g_env, k);
	if (idx < 0)
		return ;
	free(e->g_env[idx]);
	ne = delete_env_array(e->g_env, idx);
	free(e->g_env);
	e->g_env = ne;
	e->environ = e->g_env;
}

static char	escape_char(char c)
{
	if (c == 'n')
		return ('\n');
	if (c == 't')
		return ('\t');
	if (c == 'r')
		return ('\r');
	if (c == '\\')
		return ('\\');
	return (c);
}

static char	next_escaped_char(const char *s, size_t *i)
{
	char	c;

	if (s[*i] == '\\' && s[*i + 1])
	{
		*i += 1;
		c = escape_char(s[*i]);
		*i += 1;
	}
	else
	{
		c = s[*i];
		*i += 1;
	}
	return (c);
}

static char	*interpret_escapes(const char *s)
{
	size_t	i;
	size_t	j;
	size_t	n;
	char	*b;
	char	c;

	i = 0;
	j = 0;
	n = ft_strlen(s) * 2 + 1;
	b = malloc(n);
	if (!b)
		fatal_error("malloc failed in interpret_escapes");
	while (s[i])
	{
		c = next_escaped_char(s, &i);
		b[j++] = c;
	}
	b[j] = '\0';
	return (b);
}

static int	parse_echo_opt(char **a, int *nl, int *esc)
{
	int	j;

	*nl = 1;
	*esc = 0;
	if (!a[1] || a[1][0] != '-' || !a[1][1])
		return (1);
	j = 1;
	while (a[1][j])
	{
		if (a[1][j] == 'n')
			*nl = 0;
		else if (a[1][j] == 'e')
			*esc = 1;
		j++;
	}
	return (2);
}

static void	echo_print(char **a, int st, int esc)
{
	int		k;
	char	*tmp;

	k = st;
	while (a[k])
	{
		if (k > st)
			printf(" ");
		if (esc)
		{
			tmp = interpret_escapes(a[k]);
			printf("%s", tmp);
			free(tmp);
		}
		else
			printf("%s", a[k]);
		k++;
	}
}

int	builtin_echo(char **a)
{
	int	nl;
	int	esc;
	int	st;

	st = parse_echo_opt(a, &nl, &esc);
	echo_print(a, st, esc);
	if (nl)
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

static int	builtin_env(char **argv, t_env *env)
{
	int	i;

	i = 0;
	(void)argv;
	while (env->environ[i])
	{
		printf("%s\n", env->environ[i]);
		i++;
	}
	return (0);
}

static int	builtin_export(char **argv, t_env *env)
{
	int		i;
	int		j;
	char	*key_val;
	char	*eq;
	char	*val;

	if (!argv[1])
	{
		j = 0;
		while (env->g_env && env->g_env[j])
		{
			printf("declare -x %s\n", env->g_env[j]);
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
			val = getenv(key_val);
			if (!val)
				val = "";
			update_env(key_val, val, env);
		}
		else
		{
			*eq = '\0';
			update_env(key_val, eq + 1, env);
			*eq = '=';
		}
		i++;
	}
	return (0);
}

static int	builtin_unset(char **argv, t_env *env)
{
	int	i;

	i = 1;
	while (argv[i])
	{
		remove_env(argv[i], env);
		unsetenv(argv[i]);
		i++;
	}
	env->environ = env->g_env;
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
