/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 17:49:56 by mawako            #+#    #+#             */
/*   Updated: 2025/04/07 18:04:09 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	free_token(t_token *tok)
{
	t_token	*tmp;

	while (tok)
	{
		tmp = tok->next;
		if (tok->word)
			free(tok->word);
		free(tok);
		tok = tmp;
	}
}

void	free_redirects(t_redirect *redir)
{
	t_redirect	*tmp;

	while (redir)
	{
		tmp = redir->next;
		free(redir->word);
		free(redir->heredoc_filename);
		free(redir);
		redir = tmp;
	}
}

void	free_node(t_node *node)
{
	if (!node)
		return ;
	free_node(node->next);
	free_redirects(node->redirects);
	free_token(node->args);
	free_token(node->filename);
	if (node->separator)
		free(node->separator);
	free(node);
}

void	free_argv(char **argv)
{
	size_t	i;

	if (!argv)
		return ;
	i = 0;
	while (argv[i])
	{
		free(argv[i]);
		i++;
	}
	free(argv);
}
