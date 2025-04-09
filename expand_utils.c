/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/09 19:08:04 by mawako            #+#    #+#             */
/*   Updated: 2025/04/09 19:12:49 by mawako           ###   ########.fr       */
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

char	*expand_exit_status(void)
{
	extern int	g_last_exit_status;

	return (ft_itoa(g_last_exit_status));
}
