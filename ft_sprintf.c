/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_sprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 17:29:06 by shuu              #+#    #+#             */
/*   Updated: 2025/05/22 13:59:56 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// simple_sprintf function

void	ft_sprintf(char *ret, const char *fmt, const char *s1, const char *s2)
{
	int	flag;
	int	i;
	int	j;

	if (!s1 || !s2 || !fmt)
		return ;
	flag = 0;
	i = 0;
	j = 0;
	while (fmt[i])
	{
		if (fmt[i] == '%' && fmt[i + 1] == 's')
		{
			flag++;
			i += 2;
			while (*s1 && flag == 1)
				ret[j++] = *s1++;
			while (*s2 && flag == 2)
				ret[j++] = *s2++;
		}
		else
			ret[j++] = fmt[i++];
	}
	ret[j] = '\0';
}

// int main(void)
// {
//     char *ret = malloc(sizeof(char) * 10);
//     ft_sprintf(ret, "%s=%s", "1000","1000");
//     printf("%s\n", ret);
// }