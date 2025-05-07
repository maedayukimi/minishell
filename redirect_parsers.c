/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_parsers.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:24:18 by mawako            #+#    #+#             */
/*   Updated: 2025/05/07 19:32:56 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redirect	*redirect_type(t_token **rest, t_token *tok, char *type)
{
	t_redirect	*r;

	r = malloc(sizeof(t_redirect));
	if (r == NULL)
		fatal_error("redirect_type: malloc");
	init_redir(r);
	if (tok->next == NULL)
		fatal_error("redirect_type: unexpected token `newline'");
	set_redir_type(r, type);
	r->word = strdup(tok->next->word);
	*rest = tok->next->next;
	return (r);
}

t_redirect	*redirect_type2(t_token **rest, t_token *tok, char *type)
{
	t_redirect	*r;
	int			h;

	r = malloc(sizeof(t_redirect));
	if (r == NULL)
		fatal_error("redirect_type2: malloc");
	init_redir(r);
	h = set_redir2_type(r, type);
	if (!h)
	{
		free(r);
		return (NULL);
	}
	r->word = NULL;
	*rest = tok->next;
	return (r);
}
