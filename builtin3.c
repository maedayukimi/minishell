/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin3.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mawako <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/26 15:59:04 by mawako            #+#    #+#             */
/*   Updated: 2025/05/26 15:59:14 by mawako           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	parse_echo_opt(char **a, int *nl, int *esc)
{
	int	j;

	*nl = 1;
	*esc = 0;
	if (!a[1] || a[1][0] != '-' || !a[1][1])
		return (1);
	j = 1;
	while (a[1][j])
	{
		if (a[1][j] == 'n')
			*nl = 0;
		else if (a[1][j] == 'e')
			*esc = 1;
		j++;
	}
	return (2);
}

void	echo_print(char **a, int st, int esc)
{
	int		k;
	char	*tmp;

	k = st;
	while (a[k])
	{
		if (k > st)
			printf(" ");
		if (esc)
		{
			tmp = interpret_escapes(a[k]);
			printf("%s", tmp);
			free(tmp);
		}
		else
			printf("%s", a[k]);
		k++;
	}
}

int	builtin_echo(char **a)
{
	int	nl;
	int	esc;
	int	st;

	st = parse_echo_opt(a, &nl, &esc);
	echo_print(a, st, esc);
	if (nl)
		printf("\n");
	return (0);
}

int	builtin_cd(char **argv)
{
	char	*home;

	if (!argv[1])
	{
		home = getenv("HOME");
		if (!home)
		{
			fprintf(stderr, "cd: HOME not set\n");
			return (1);
		}
		if (chdir(home) != 0)
		{
			perror("cd");
			return (1);
		}
		return (0);
	}
	if (chdir(argv[1]) != 0)
	{
		perror("cd");
		return (1);
	}
	return (0);
}

int	builtin_pwd(void)
{
	char	cwd[PATH_MAX];

	if (!getcwd(cwd, sizeof(cwd)))
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", cwd);
	return (0);
}

