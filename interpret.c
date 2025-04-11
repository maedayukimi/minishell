/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   interpret.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 17:20:46 by mawako            #+#    #+#             */
/*   Updated: 2025/04/11 17:12:44 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	interpret(char *line, int *status)
{
	t_token	*words;
	t_node	*node;

	words = tokenize(line);
	if (!words || words->kind == TK_EOF)
	{
		*status = ERROR_TOKENIZE;
		free_token(words);
		return ;
	}
	node = parse(words);
	free_token(words);
	if (!node)
	{
		*status = 258;
		return ;
	}
	expansion(node);
	setup_heredoc(node);
	*status = exec_tree(node);
	free_node(node);
}
