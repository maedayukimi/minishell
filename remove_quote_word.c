/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   remove_quote_word.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: shuu <shuu@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 16:10:41 by shuu              #+#    #+#             */
/*   Updated: 2025/05/21 16:11:44 by shuu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	removed_quote_word(char *new, char *word)
{
	int		i;
	int		j;
	char	quote;

	i = 0;
	j = 0;
	while (word[i])
	{
		if (word[i] == '\'' || word[i] == '\"')
		{
			quote = word[i++];
			while (word[i] && word[i] != quote)
				new[j++] = word[i++];
			if (word[i] == quote)
				i++;
		}
		else
			new[j++] = word[i++];
	}
	new[j] = '\0';
}

char	*remove_quote_word(char *word)
{
	char	*new;

	if (!word)
		return (NULL);
	new = malloc(ft_strlen(word) + 1);
	if (!new)
		fatal_error("remove_quote_word: malloc");
	removed_quote_word(new, word);
	free(word);
	return (new);
}
