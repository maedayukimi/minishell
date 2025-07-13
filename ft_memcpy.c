/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/24 12:15:55 by mawako            #+#    #+#             */
/*   Updated: 2025/07/13 15:38:09 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char	*d;

	if (!dest && !src)
		return (NULL);
	d = (unsigned char *)dest;
	while (n--)
	{
		*d = (*(unsigned char *)src);
		d++;
		src++;
	}
	return (dest);
}
