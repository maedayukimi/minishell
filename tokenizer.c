/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 16:05:57 by mawako            #+#    #+#             */
/*   Updated: 2025/03/28 17:21:55 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*tokenize(char *line)
{
	t_token	head;
	t_token	*cur;

	head.next = NULL;
	cur = &head;
	while (*line)
	{
		if (consume_blank(&line, line))
			continue ;
		else if (is_metacharacter(*line) || check_redirect(line))
		{
			cur->next = operator_func(&line, line);
			cur = cur->next;
		}
		else
		{
			cur->next = word_func(&line, line);
			cur = cur->next;
		}
	}
	cur->next = new_token(NULL, TK_EOF);
	return (head.next);
}
