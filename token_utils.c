/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:17 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 21:56:04 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	return (ft_strncmp(s, keyword, ft_strlen(keyword)) == 0);
}

int	check_redirect(const char *s)
{
	if (!s || !*s)
		return (0);
	if (isdigit((unsigned char)s[0]) && (s[1] == '<' || s[1] == '>'))
		return (1);
	return (0);
}

void	tokenize_error(const char *location, char **rest, char *line)
{
	ft_dprintf(STDERR_FILENO,
		"minishell: syntax error near %s\n", location, NULL);
	while (*line)
		line++;
	*rest = line;
}
