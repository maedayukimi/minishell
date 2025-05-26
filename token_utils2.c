/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 21:56:11 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 21:56:27 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_metacharacter(char c)
{
	return (c && strchr("|&;()<> \t\n", c));
}

int	is_word(const char *s)
{
	return (*s && !is_metacharacter(*s));
}

int	is_blank(char c)
{
	return ((c == ' ') || (c == '\t') || (c == '\n'));
}
