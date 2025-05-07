/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect_init.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/07 19:20:51 by mawako            #+#    #+#             */
/*   Updated: 2025/05/07 19:22:06 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_redir(t_redirect *r)
{
	r->next = NULL;
	r->quoted = 0;
	r->heredoc_expand = 0;
	r->heredoc_filename = NULL;
	r->filefd = 0;
	r->stashed_targetfd = 0;
	r->targetfd2 = 0;
	r->stashed_targetfd2 = 0;
}
