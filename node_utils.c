/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/15 16:14:53 by mawako            #+#    #+#             */
/*   Updated: 2025/05/20 17:49:43 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_node	*new_node(t_node_kind kind)
{
	t_node	*node;

	node = malloc(sizeof(t_node));
	if (!node)
		fatal_error("new_node: malloc");
	node->kind = kind;
	node->args = NULL;
	node->filename = NULL;
	node->next = NULL;
	node->redirects = NULL;
	node->separator = NULL;
	node->targetfd = 0;
	node->targetfd2 = 0;
	node->filefd = 0;
	node->stashed_targetfd = 0;
	node->stashed_targetfd2 = 0;
	node->child = NULL;
	return (node);
}

t_token	*tokdup(t_token *tok)
{
	char	*word;

	if (!tok)
		return (NULL);
	word = ft_strdup(tok->word);
	if (!word)
		fatal_error("tokdup: ft_strdup");
	return (new_token(word, tok->kind));
}
