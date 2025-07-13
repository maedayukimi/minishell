/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_complete_input.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 15:24:12 by shuu              #+#    #+#             */
/*   Updated: 2025/05/21 16:37:41 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

extern int	g_sig_subshell;

static int	line_ends_with_pipe(const char *line)
{
	size_t	i;

	i = ft_strlen(line);
	while (i > 0 && (line[i - 1] == '\n'
			|| ft_isspace((unsigned char)line[i - 1])))
		i--;
	if (i > 0 && line[i - 1] == '|')
		return (1);
	return (0);
}

static int	ends_with_bs(const char *line, size_t *count)
{
	size_t	i;
	size_t	c;

	i = ft_strlen(line);
	while (i > 0 && (line[i - 1] == '\n'
			|| ft_isspace((unsigned char)line[i - 1])))
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

	prefix = ft_strlen(line);
	while (prefix > 0 && (line[prefix - 1] == '\n'
			|| ft_isspace((unsigned char)line[prefix - 1])))
		prefix--;
	prefix -= bs_count;
	new = malloc(prefix + bs_count - 1 + ft_strlen(more) + 1);
	if (new == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < bs_count - 1)
	{
		new[prefix + i] = '\\';
		i++;
	}
	ft_memcpy(new, line, prefix);
	ft_memcpy(new + prefix + i, more, ft_strlen(more));
	new[prefix + i + ft_strlen(more)] = '\0';
	free(line);
	free(more);
	return (new);
}

static char	*join_pipe_line(char *line, char *more)
{
	char	*new;
	size_t	len;
	size_t	i;

	len = ft_strlen(line);
	new = malloc(len + 1 + ft_strlen(more) + 1);
	if (new == NULL)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < len)
	{
		new[i] = line[i];
		i++;
	}
	new[i++] = '\n';
	ft_memcpy(new + i, more, ft_strlen(more));
	new[i + ft_strlen(more)] = '\0';
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
		g_sig_subshell = 1;
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
