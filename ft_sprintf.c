/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:29:06 by shuu              #+#    #+#             */
/*   Updated: 2025/05/21 21:19:14 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// simple_sprintf function

static void	add_string(char *ret, int *j, const char *s1, const char *s2)
{
	if (*s1)
	{
		while (*s1)
			ret[(*j)++] = *s1++;
	}
	else if (*s2)
	{
		while (*s2)
			ret[(*j)++] = *s2++;
	}
	else
	{
		free(ret);
		ret = NULL;
	}
}

void	ft_sprintf(char *ret, const char *fmt, const char *s1, const char *s2)
{
	int	i;
	int	j;

	if (!s1 || !*s1 || !s2 || !*s2)
		return ;
	i = 0;
	j = 0;
	while (fmt[i])
	{
		if (fmt[i] == '%' && fmt[i + 1] == 's')
		{
			i += 2;
			add_string(ret, &j, s1, s2);
			if (!ret)
				break ;
		}
		else
			ret[j++] = fmt[i++];
	}
	ret[j] = '\0';
}
