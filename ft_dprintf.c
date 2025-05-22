/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/22 12:11:24 by shuu              #+#    #+#             */
/*   Updated: 2025/05/22 13:49:26 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// simple_dprintf function

void	s_output(int fd, const char *fmt, const char *s1, const char *s2)
{
	int	flag;
	int	i;
    int j;

	flag = 0;
	i = 0;
	while (fmt[i])
	{
		if (fmt[i] == '%' && fmt[i + 1] == 's')
		{
            j = 0;
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

// int main(void)
// {
//     char s1[] = "asdf";
//     ft_dprintf(1);
// }