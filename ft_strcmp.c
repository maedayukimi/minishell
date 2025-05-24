/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/24 21:35:21 by shuu              #+#    #+#             */
/*   Updated: 2025/05/24 21:42:17 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_strcmp(const char *str1, const char *str2)
{
	size_t			i;
	unsigned char	*s1_cpy;
	unsigned char	*s2_cpy;

	s1_cpy = (unsigned char *)str1;
	s2_cpy = (unsigned char *)str2;
	i = 0;
	while (s1_cpy[i] && s2_cpy[i])
	{
		if (s1_cpy[i] != s2_cpy[i])
			return (s1_cpy[i] - s2_cpy[i]);
		i++;
	}
	return (s1_cpy[i] - s2_cpy[i]);
}
