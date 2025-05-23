/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:11:24 by shuu              #+#    #+#             */
/*   Updated: 2025/05/23 18:15:19 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	s_output(int fd, const char *fmt, const char *s1, const char *s2)
{
	int	flag;
	int	i;

	flag = 0;
	i = 0;
	while (fmt[i])
	{
		if (fmt[i] == '%' && fmt[i + 1] == 's')
		{
			flag++;
			i += 2;
			if (flag == 1)
				write(fd, s1, ft_strlen(s1));
			else
				write(fd, s2, ft_strlen(s2));
		}
		else
			write(fd, &fmt[i++], 1);
	}
}

void	ft_dprintf(int fd, const char *fmt, const char *s1, const char *s2)
{
	int	i;

	i = 0;
	if (!fmt)
		return ;
	if (!s1 && !s2 && fmt)
	{
		while (fmt[i])
			write(fd, &fmt[i++], 1);
	}
	else
		s_output(fd, fmt, s1, s2);
}
