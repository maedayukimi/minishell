/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate_argv.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:23 by mawako            #+#    #+#             */
/*   Updated: 2025/03/31 17:54:30 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**allocate_argv(t_token *tok, size_t size)
{
	char	**heap;
	size_t	i;

	heap = (char **)malloc(sizeof(char *) * (size + 1));
	if (!heap)
		return (NULL);
	i = 0;
	while (tok && i < size)
	{
		heap[i] = (char *)malloc(sizeof(char) * (strlen(tok->word) + 1));
		if (!heap[i])
		{
			while (i > 0)
				free(heap[--i]);
			free(heap);
			return (NULL);
		}
		strcpy(heap[i], tok->word);
		tok = tok->next;
		i++;
	}
	heap[i] = NULL;
	return (heap);
}

char	**create_argv(t_token *tok)
{
	size_t	size;
	t_token	*tmp;
	char	**argv;

	size = 0;
	tmp = tok;
	while (tmp && tmp->word)
	{
		size++;
		tmp = tmp->next;
	}
	argv = allocate_argv(tok, size);
	return (argv);
}
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_word.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:04:39 by mawako            #+#    #+#             */
/*   Updated: 2025/03/31 14:43:34 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*new_token(char *word, t_token_kind kind)
{
	t_token	*tok;

	tok = (t_token *)malloc(sizeof(t_token));
	if (!tok)
		fatal_error("new_token: malloc");
	tok->word = word;
	tok->kind = kind;
	tok->next = NULL;
	return (tok);
}

t_token	*operator_func(char **rest, char *line)
{
	size_t				i;
	char				*op;
	static char *const	operators[] = {
		"1>&1", "1>&2", ">&1", "2>&2", ">&2", "2>&1", "2>", "2>>", "1>", "1>>",
		"&>", "&>>", ">>", "<<", ">", "<", "||", "&&", "&", ";", ";;", "(",
		")", "|", "\n", NULL
	};

	i = 0;
	while (operators[i])
	{
		if (startswith(line, operators[i]))
		{
			op = strdup(operators[i]);
			if (!op)
				fatal_error("operator_func: strdup");
			*rest = line + strlen(op);
			return (new_token(op, TK_OP));
		}
		i++;
	}
	fatal_error("Unexpected operator");
	return (NULL);
}

t_token	*word_func(char **rest, char *line)
{
	int		in_squote;
	int		in_dquote;
	char	*start;
	char	*tmp;

	in_squote = 0;
	in_dquote = 0;
	start = line;
	while (*line)
	{
		if (*line == SINGLE_QUOTE_CHAR && !in_dquote)
			in_squote = !in_squote;
		else if (*line == DUOBLE_QUOTE_CHAR && !in_squote)
			in_dquote = !in_dquote;
		if (!in_squote && !in_dquote)
		{
			if (is_blank(*line) || is_metacharacter(*line)
				|| check_redirect(line))
				break ;
		}
		line++;
	}
	tmp = strndup(start, line - start);
	if (!tmp)
		fatal_error("word_func: strndup");
	*rest = line;
	return (new_token(tmp, TK_WORD));
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 17:22:06 by mawako            #+#    #+#             */
/*   Updated: 2025/03/28 17:57:01 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "env_management.h"

char	**g_env = NULL;

void	init_env(char **envp)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	while (envp[count])
		count++;
	g_env = malloc((count + 1) * sizeof(char *));
	if (!g_env)
	{
		perror("malloc");
		exit(1);
	}
	while (i < count)
	{
		g_env[i] = strdup(envp[i]);
		if (!g_env[i++])
		{
			perror("strdup");
			exit(1);
		}
	}
	g_env[count] = (NULL);
}

char	*my_getenv(const char *name)
{
	int		i;
	size_t	len;

	i = 0;
	if (!name || !g_env)
		return (NULL);
	len = strlen(name);
	while (g_env[i])
	{
		if (strncmp(g_env[i], name, len) == 0 && g_env[i][len] == '=')
			return (g_env[i] + len + 1);
		i++;
	}
	return (NULL);
}

int	my_setenv(const char *name, const char *value, int overwrite)
{
	int		i;
	size_t	len;
	char	*new_entry;
	int		count;
	char	**new_env;

	i = 0;
	count = 0;
	if (!name || strchr(name, '='))
		return (-1);
	len = strlen(name);
	if (g_env)
	{
		while (g_env[i])
		{
			count++;
			if (strncmp(g_env[i], name, len) == 0 && g_env[i][len] == '=')
			{
				if (!overwrite)
					return (0);
				new_entry = malloc(len + strlen(value) + 2);
				if (!new_entry)
					return (-1);
				sprintf(new_entry, "%s=%s", name, value);
				free(g_env[i]);
				g_env[i] = new_entry;
				return (0);
			}
			i++;
		}
	}
	{
		new_env = realloc(g_env, (count + 2) * sizeof(char *));
		if (!new_env)
			return (-1);
		g_env = new_env;
		new_entry = malloc(len + strlen(value) + 2);
		if (!new_entry)
			return (-1);
		sprintf(new_entry, "%s=%s", name, value);
		g_env[count] = new_entry;
		g_env[count + 1] = NULL;
		return (0);
	}
}

int	my_unsetenv(const char *name)
{
	int		i;
	int		j;
	size_t	len;

	i = 0;
	j = 0;
	if (!name || !g_env)
		return (-1);
	len = strlen(name);
	while (g_env[i])
	{
		if (strncmp(g_env[i], name, len) == 0 && g_env[i][len] == '=')
		{
			free(g_env[i]);
			while (g_env[j])
			{
				g_env[j] = g_env[j + 1];
				j++;
			}
			i--;
		}
	}
	return (0);
}

void	free_env(void)
{
	int	i;

	i = 0;
	if (!g_env)
		return ;
	while (g_env[i])
		free(g_env[i++]);
	free(g_env);
	g_env = (NULL);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_variables.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/31 18:14:12 by mawako            #+#    #+#             */
/*   Updated: 2025/03/31 18:29:22 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_char_and_free(char *s, char c)
{
	int		len;
	char	*res;

	len = strlen(s);
	res = malloc(len + 2);
	if (!res)
		return (NULL);
	memcpy(res, s, len);
	res[len] = c;
	res[len + 1] = '\0';
	free(s);
	return (res);
}

char	*ft_strjoin_and_free(char *s1, char *s2)
{
	char	*res;

	res = ft_strjoin(s1, s2);
	free(s1);
	free(s2);
	return (res);
}

int	is_var_char(char c)
{
	return (isalnum(c) || c == '_');
}

static char	*get_env_value(const char *name)
{
	extern char	**environ;
	int			i;
	int			len;

	len = strlen(name);
	i = 0;
	while (environ[i])
	{
		if (strncmp(environ[i], name, len) == 0 && environ[i][len] == '=')
			return (environ[i] + len + 1);
		i++;
	}
	return ("");
}

static char	*expand_exit_status(void)
{
	extern int	g_last_exit_status;
	return (ft_itoa(g_last_exit_status));
}

char	*expand_variables(char *str)
{
	char	*result;
	int		i;
	char	*status;
	int		start;
	char	*key;
	char	*val;

	result = calloc(1, 1);
	if (!result)
		return (NULL);
	i = 0;
	while (str[i])
	{
		if (str[i] == '$')
		{
			if (str[i + 1] == '?')
			{
				status = expand_exit_status();
				result = ft_strjoin_and_free(result, status);
				i += 2;
			}
			else if (is_var_char(str[i + 1]))
			{
				start = ++i;
				while (is_var_char(str[i]))
					i++;
				key = ft_substr(str, start, i - start);
				val = get_env_value(key);
				result = ft_strjoin_and_free(result, strdup(val));
				free(key);
			}
			else
			{
				result = ft_strjoin_char_and_free(result, str[i]);
				i++;
			}
		}
		else
		{
			result = ft_strjoin_char_and_free(result, str[i]);
			i++;
		}
	}
	return (result);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:27 by mawako            #+#    #+#             */
/*   Updated: 2025/03/31 18:31:46 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_exit_status;
extern int	g_last_exit_status;

void	append_char(char **s, char c)
{
	char	*new;
	size_t	size;

	size = 2;
	if (*s)
		size += strlen(*s);
	new = malloc(size);
	if (!new)
		fatal_error("append_char: malloc");
	if (*s)
	{
		strlcpy(new, *s, size);
		free(*s);
	}
	new[size - 2] = c;
	new[size - 1] = '\0';
	*s = new;
}

char	*find_env(char *p)
{
	char	*env;
	char	*content;
	size_t	size;

	p++;
	size = 0;
	while (p[size] && (p[size] != DUOBLE_QUOTE_CHAR)
		&& (p[size] != SINGLE_QUOTE_CHAR) && !is_blank(p[size]))
		size++;
	env = strndup(p, size);
	if (!env)
		fatal_error("find_env: strndup");
	content = getenv(env);
	free(env);
	if (!content)
		return (NULL);
	return (content);
}

void	remove_quote(t_token *tok)
{
	char	*new_word;
	char	*p;
	int		i;
	size_t	j;

	if (!tok || tok->kind != TK_WORD || !tok->word)
		return ;
	p = tok->word;
	new_word = NULL;
	i = 0;
	while (p[i])
	{
		if (p[i] == SINGLE_QUOTE_CHAR || p[i] == DUOBLE_QUOTE_CHAR)
		{
			i++;
			while (p[i] && p[i] != SINGLE_QUOTE_CHAR && p[i] != DUOBLE_QUOTE_CHAR)
				append_char(&new_word, p[i++]);
			if (p[i])
				i++;
		}
		else if (p[i] == '$')
		{
			if (p[i + 1] == '?')
			{
				char *status = ft_itoa(g_last_exit_status);
				if (status)
				{
					j = 0;
					while (status[j])
						append_char(&new_word, status[j++]);
					free(status);
				}
				i += 2;
			}
			else if (is_var_char(p[i + 1]))
			{
				char *tmp;
				tmp = find_env(&p[i]);
				if (tmp)
				{
					j = 0;
					while (tmp[j])
						append_char(&new_word, tmp[j++]);
				}
				while (p[i] && !is_blank(p[i]) &&
					p[i] != SINGLE_QUOTE_CHAR && p[i] != DUOBLE_QUOTE_CHAR)
					i++;
			}
			else
				append_char(&new_word, p[i++]);
		}
		else
			append_char(&new_word, p[i++]);
	}
	free(tok->word);
	tok->word = new_word;
	remove_quote(tok->next);
}

char	*remove_quote_word(char *word)
{
	char	*new;
	int		i;
	int		j;
	char	quote;

	if (!word)
		return (NULL);
	new = malloc(strlen(word) + 1);
	if (!new)
		fatal_error("remove_quote_word: malloc");
	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '\"')
		{
			quote = word[i++];
			while (word[i] && word[i] != quote)
				new[j++] = word[i++];
			if (word[i] == quote)
				i++;
		}
		else
			new[j++] = word[i++];
	}
	new[j] = '\0';
	free(word);
	return (new);
}

void	quote_removal(t_node *node)
{
	if (!node)
		return ;
	if (node->args)
		remove_quote(node->args);
	if (node->filename)
		remove_quote(node->filename);
	quote_removal_redirects(node->redirects);
	if (node->next)
		quote_removal(node->next);
}

void	quote_removal_redirects(t_redirect *redir)
{
	char	*new_word;

	while (redir)
	{
		if (redir->word)
		{
			new_word = remove_quote_word(redir->word);
			redir->word = new_word;
		}
		redir = redir->next;
	}
}

void	expansion(t_node *node)
{
	quote_removal(node);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:49:56 by mawako            #+#    #+#             */
/*   Updated: 2025/03/26 18:50:55 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *tok)
{
	t_token	*tmp;

	while (tok)
	{
		tmp = tok->next;
		if (tok->word)
			free(tok->word);
		free(tok);
		tok = tmp;
	}
}

void	free_redirects(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->word);
		free(redir->heredoc_filename);
		free(redir);
		redir = tmp;
	}
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	free_node(node->next);
	free_redirects(node->redirects);
	free_token(node->args);
	free_token(node->filename);
	if (node->separator)
		free(node->separator);
	free(node);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/21 16:06:04 by mawako            #+#    #+#             */
/*   Updated: 2025/03/31 18:12:35 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static long int	ft_len_long(long int nb)
{
	long int	len;

	len = 0;
	if (nb <= 0)
		len = 1;
	while (nb)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	long int	nb;
	long int	len;
	char		*s;

	nb = (long int)n;
	len = ft_len_long(nb);
	s = malloc(sizeof(char) * (len + 1));
	if (!s)
		return (NULL);
	s[len] = '\0';
	if (nb < 0)
	{
		s[0] = '-';
		nb = -nb;
	}
	if (nb == 0)
		s[0] = '0';
	while (nb > 0)
	{
		s[--len] = (nb % 10) + '0';
		nb /= 10;
	}
	return (s);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 06:37:23 by mawako            #+#    #+#             */
/*   Updated: 2025/03/26 16:30:37 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t	count(char const *s, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == 0))
			count++;
		i++;
	}
	return (count);
}

static size_t	split_words(char **res, char const *s, char c, size_t word)
{
	size_t	i;
	size_t	j;

	j = 0;
	i = 0;
	while (s[j])
	{
		if (s[j] == c || s[j] == 0)
			i = j + 1;
		if (s[j] != c && (s[j + 1] == c || s[j + 1] == 0))
		{
			res[word] = malloc(sizeof(char) * (j - i + 2));
			if (!res[word])
			{
				while (word++)
					free(res[word]);
				return (0);
			}
			strlcpy(res[word], (s + i), j - i + 2);
			word++;
		}
		j++;
	}
	res[word] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**res;

	if (!s)
		return (0);
	res = malloc(sizeof(char *) * (count(s, c) + 1));
	if (!res)
		return (0);
	if (!split_words(res, s, c, 0))
		return (free(res), NULL);
	return (res);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:04:50 by mawako            #+#    #+#             */
/*   Updated: 2025/03/24 15:21:48 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(const char *s1, const char *s2)
{
	char	*join;
	size_t	i;
	size_t	j;
	size_t	len1;
	size_t	len2;

	if (!s1 || !s2)
		return (NULL);
	len1 = strlen(s1);
	len2 = strlen(s2);
	join = (char *)malloc(sizeof(char) * (len1 + len2 + 1));
	if (!join)
		return (NULL);
	i = 0;
	while (i < len1)
	{
		join[i] = s1[i];
		i++;
	}
	j = 0;
	while (j < len2)
		join[i++] = s2[j++];
	join[i] = '\0';
	return (join);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_substr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:30:29 by mawako            #+#    #+#             */
/*   Updated: 2025/03/26 18:27:11 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	size_t	i;
	char	*mem;

	i = 0;
	if (!s)
		return (NULL);
	if (start >= strlen(s))
	{
		mem = (char *)malloc(1);
		if (mem)
			mem[0] = '\0';
		return (mem);
	}
	if (len > strlen(s) - start)
		len = strlen(s) - start;
	mem = (char *)malloc((len + 1) * sizeof(char));
	if (!mem)
		return (NULL);
	while (i < len)
		mem[i++] = s[start++];
	mem[i] = '\0';
	return (mem);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 18:05:29 by mawako            #+#    #+#             */
/*   Updated: 2025/03/28 18:47:31 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	expand_and_write(int fd, char *line, int expand)
{
	char	*expanded;

	if (expand)
		expanded = expand_variables(line);
	else
		expanded = strdup(line);
	if (expanded)
	{
		write(fd, expanded, strlen(expanded));
		write(fd, "\n", 1);
		free(expanded);
	}
}

int	read_heredoc(const char *delimiter, int expand, char *heredoc_file)
{
	char	*line;
	int		fd;

	fd = open(heredoc_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd < 0)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		expand_and_write(fd, line, expand);
		free(line);
	}
	close(fd);
	return (0);
}

int	setup_heredoc(t_node *node)
{
	t_redirect	*redir;
	static int	heredoc_count;
	char		filename[256];

	redir = node->redirects;
	heredoc_count = 0;
	while (redir)
	{
		if (redir->type == R_HEREDOC)
		{
			snprintf(filename, sizeof(filename),
				"/tmp/.heredoc_%d", heredoc_count++);
			redir->heredoc_filename = strdup(filename);
			redir->heredoc_expand = !redir->quoted;
			read_heredoc(redir->word, redir->heredoc_expand, filename);
		}
		redir = redir->next;
	}
	return (0);
}
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
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:04:59 by mawako            #+#    #+#             */
/*   Updated: 2025/04/03 15:23:19 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = (t_node *)malloc(sizeof(t_node));
	if (!node)
		fatal_error("new_node: malloc");
	node->kind = kind;
	node->args = NULL;
	node->filename = NULL;
	node->next = NULL;
	node->redirects = NULL;
	node->separator = NULL;
	node->targetfd = 0;
	node->targetfd2 = 0;
	node->filefd = 0;
	node->stashed_targetfd = 0;
	node->stashed_targetfd2 = 0;
	return (node);
}

t_token	*tokdup(t_token *tok)
{
	char	*word;

	if (!tok)
		return (NULL);
	word = strdup(tok->word);
	if (!word)
		fatal_error("tokdup: strdup");
	return (new_token(word, tok->kind));
}

void	append_redirect(t_redirect **list, t_redirect *new_redir)
{
	t_redirect	*cur;

	if (!list || !new_redir)
		return ;
	if (*list == NULL)
		*list = new_redir;
	else
	{
		cur = *list;
		while (cur->next)
			cur = cur->next;
		cur->next = new_redir;
	}
}

static void	append_node(t_node **nodes, t_node *node)
{
	if (!*nodes)
	{
		*nodes = node;
		return ;
	}
	append_node(&(*nodes)->next, node);
}

void	append_tok(t_token **tokens, t_token *tok)
{
	if (!*tokens)
	{
		*tokens = tok;
		return ;
	}
	append_tok(&(*tokens)->next, tok);
}

static int	is_redirect(const char *word)
{
	int			i;
	static char *const	redirs[] = {
		">>", "2>>", "<<", "<", ">", "2>", "1>", ">&1", "1>&1",
		"1>&2", ">&2", "2>&2", "2>&1", "&>", ">&", NULL
	};

	i = 0;
	if (!word)
		return (0);
	while (redirs[i])
	{
		if (!strcmp(redirs[i], word))
			return (1);
		i++;
	}
	return (0);
}

static int	is_redirect_op(t_token *tok)
{
	if (!tok || tok->kind != TK_OP)
		return (0);
	return (is_redirect(tok->word));
}

static int	is_control_operator(t_token *tok)
{
	if (!tok || tok->kind != TK_OP)
		return (0);
	if (!strcmp(tok->word, ";"))
		return (1);
	if (!strcmp(tok->word, "&&"))
		return (1);
	if (!strcmp(tok->word, "||"))
		return (1);
	if (!strcmp(tok->word, "|"))
		return (1);
	if (!strcmp(tok->word, "&"))
		return (1);
	return (0);
}

static t_node	*parse_simple_command(t_token **rest)
{
	t_node	*node;
	t_token	*tok;

	tok = *rest;
	node = new_node(ND_SIMPLE_CMD);
	while (tok && tok->kind != TK_EOF && !is_control_operator(tok))
	{
		if (tok->kind == TK_WORD)
		{
			append_tok(&node->args, tokdup(tok));
			tok = tok->next;
		}
		else if (is_redirect_op(tok))
		{
			if (!tok->next || tok->next->kind != TK_WORD)
			{
				fprintf(stderr, "minishell: syntax error near unexpected token `newline'\n");
				return (NULL);
			}
			append_redirect(&node->redirects, redirect_type(&tok, tok, tok->word));
		}
		else
			fatal_error("parse_simple_command: unexpected token");
	}
	*rest = tok;
	return (node);
}

t_node	*parse(t_token *tok)
{
	t_node	*head;
	t_node	*cur;
	t_node	*next_cmd;

	head = NULL;
	cur = NULL;
	while (tok && tok->kind != TK_EOF)
	{
		next_cmd = parse_simple_command(&tok);
		if (!next_cmd)
			return (NULL);
		if (!head)
			head = next_cmd;
		else
			append_node(&cur->next, next_cmd);
		cur = next_cmd;
		if (is_control_operator(tok))
		{
			cur->separator = strdup(tok->word);
			tok = tok->next;
		}
	}
	return (head);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:46 by mawako            #+#    #+#             */
/*   Updated: 2025/03/28 17:19:45 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	open_redir_file(t_redirect *redir)
{
	int		flags;
	mode_t	mode;

	while (redir)
	{
		flags = 0;
		mode = 0644;
		if (redir->type == ND_REDIR_OUT
			|| redir->type == ND_REDIR_OUT_ERR
			|| redir->type == ND_REDIR_SP)
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		else if (redir->type == ND_REDIR_IN || redir->type == R_HEREDOC)
			flags = O_RDONLY;
		else if (redir->type == ND_REDIR_APPEND_OUT)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else if (redir->type == ND_REDIR_APPEND_IN)
			flags = O_RDONLY | O_APPEND;
		if (redir->type == R_HEREDOC)
			redir->filefd = open(redir->heredoc_filename, O_RDONLY);
		else if (redir->word)
			redir->filefd = open(redir->word, flags, mode);
		redir = redir->next;
	}
}

void	do_redirect(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == ND_REDIR_OUT
			|| redir->type == ND_REDIR_IN
			|| redir->type == ND_REDIR_APPEND_OUT)
		{
			redir->stashed_targetfd = dup(redir->targetfd);
			dup2(redir->filefd, redir->targetfd);
		}
		else if (redir->type == ND_REDIR_SP && !redir->next)
		{
			redir->stashed_targetfd = dup(redir->targetfd);
			dup2(redir->filefd, redir->targetfd);
		}
		else if (redir->type == ND_REDIR_OUT_ERR)
		{
			redir->stashed_targetfd = dup(redir->targetfd);
			redir->stashed_targetfd2 = dup(redir->targetfd2);
			dup2(redir->filefd, redir->targetfd);
			dup2(redir->filefd, redir->targetfd2);
		}
		else if (redir->type == R_HEREDOC)
		{
			redir->stashed_targetfd = dup(redir->targetfd);
			dup2(redir->filefd, redir->targetfd);
		}
		redir = redir->next;
	}
}

void	reset_redirect(t_redirect *redir)
{
	while (redir)
	{
		if (redir->type == ND_REDIR_IN || redir->type == ND_REDIR_OUT
			|| redir->type == ND_REDIR_APPEND_OUT || redir->type == ND_REDIR_APPEND_IN
			|| redir->type == R_HEREDOC)
		{
			close(redir->filefd);
			dup2(redir->stashed_targetfd, redir->targetfd);
			close(redir->stashed_targetfd);
		}
		else if (redir->type == ND_REDIR_SP && !redir->next)
		{
			close(redir->filefd);
			dup2(redir->stashed_targetfd, redir->targetfd);
			close(redir->stashed_targetfd);
		}
		else if (redir->type == ND_REDIR_OUT_ERR && redir->filefd >= 0)
		{
			close(redir->filefd);
			dup2(redir->stashed_targetfd, redir->targetfd);
			dup2(redir->stashed_targetfd2, redir->targetfd2);
			close(redir->stashed_targetfd);
			close(redir->stashed_targetfd2);
		}
		redir = redir->next;
	}
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_type.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:06 by mawako            #+#    #+#             */
/*   Updated: 2025/03/28 19:41:09 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*redirect_type(t_token **rest, t_token *tok, char *type)
{
	t_redirect	*redir;

	redir = malloc(sizeof(t_redirect));
	if (!redir)
		fatal_error("redirect_type: malloc");
	redir->next = NULL;
	redir->quoted = 0;
	redir->heredoc_expand = 0;
	redir->heredoc_filename = NULL;
	redir->filefd = 0;
	redir->stashed_targetfd = 0;
	redir->targetfd2 = 0;
	redir->stashed_targetfd2 = 0;
	if (!tok->next)
		fatal_error("redirect_type: unexpected token `newline'");
	if (!strcmp(type, "<<"))
	{
		redir->type = R_HEREDOC;
		redir->targetfd = STDIN;
	}
	else if (!strcmp(type, "<"))
	{
		redir->type = ND_REDIR_IN;
		redir->targetfd = STDIN;
	}
	else if (!strcmp(type, ">"))
	{
		redir->type = ND_REDIR_OUT;
		redir->targetfd = STDOUT;
	}
	else if (!strcmp(type, ">>"))
	{
		redir->type = ND_REDIR_APPEND_OUT;
		redir->targetfd = STDOUT;
	}
	else if (!strcmp(type, "2>"))
	{
		redir->type = ND_REDIR_OUT;
		redir->targetfd = STDERR;
	}
	else if (!strcmp(type, "2>>"))
	{
		redir->type = ND_REDIR_APPEND_OUT;
		redir->targetfd = STDERR;
	}
	redir->word = strdup(tok->next->word);
	*rest = tok->next->next;
	return (redir);
}

t_node	*redirect_type2(t_token **rest, t_token *tok, char *type)
{
	t_node	*node;

	if (strcmp(type, ">&1") == 0 || strcmp(type, "1>&1") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDOUT;
		node->filefd = STDOUT;
	}
	else if (strcmp(type, ">&2") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDOUT;
		node->filefd = STDERR;
	}
	else if (strcmp(type, "2>&1") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDERR;
		node->filefd = STDOUT;
	}
	else if (strcmp(type, "1>&2") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDOUT;
		node->filefd = STDERR;
	}
	else if (strcmp(type, "2>&2") == 0)
	{
		node = new_node(ND_REDIR_SP);
		node->targetfd = STDERR;
		node->filefd = STDERR;
	}
	else
		return (NULL);
	node->filename = NULL;
	*rest = tok->next;
	return (node);
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:17 by mawako            #+#    #+#             */
/*   Updated: 2025/03/27 19:08:22 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_blank(char c)
{
	return ((c == ' ') || (c == '\t') || (c == '\n'));
}

int	consume_blank(char **rest, char *line)
{
	if (is_blank(*line))
	{
		while (*line && is_blank(*line))
			line++;
		*rest = line;
		return (1);
	}
	*rest = line;
	return (0);
}

int	startswith(const char *s, const char *keyword)
{
	if (!s || !keyword)
		return (0);
	return (strncmp(s, keyword, strlen(keyword)) == 0);
}

int	check_redirect(const char *s)
{
	if (!s || !*s)
		return (0);
	if (isdigit((unsigned char)s[0]) && (s[1] == '<' || s[1] == '>'))
		return (1);
	return (0);
}

int	is_metacharacter(char c)
{
	return (c && strchr("|&;()<> \t\n", c));
}

int	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

void	tokenize_error(const char *location, char **rest, char *line)
{
	dprintf(STDERR_FILENO, "minishell: syntax error near %s\n", location);
	while (*line)
		line++;
	*rest = line;
}
/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:57 by mawako            #+#    #+#             */
/*   Updated: 2025/03/28 17:21:55 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize(char *line)
{
	t_token	head;
	t_token	*cur;

	head.next = NULL;
	cur = &head;
	while (*line)
	{
		if (consume_blank(&line, line))
			continue ;
		else if (is_metacharacter(*line) || check_redirect(line))
		{
			cur->next = operator_func(&line, line);
			cur = cur->next;
		}
		else
		{
			cur->next = word_func(&line, line);
			cur = cur->next;
		}
	}
	cur->next = new_token(NULL, TK_EOF);
	return (head.next);
}
