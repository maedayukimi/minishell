/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_management.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/28 18:39:47 by mawako            #+#    #+#             */
/*   Updated: 2025/04/03 15:32:45 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ENV_MANAGEMENT_H
# define ENV_MANAGEMENT_H
# include <stdlib.h>
# include <string.h>
# include <stdio.h>

extern char	**g_env;
void	init_env(char **envp);
char	*my_getenv(const char *name);
int		my_setenv(const char *name, const char *value, int overwrite);
int		my_unsetenv(const char *name);
void	free_env(void);

#endif
