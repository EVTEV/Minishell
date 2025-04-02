/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:34:19 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/02 18:34:20 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_flag(char *s)
{
	int	i;

	i = 0;
	if (s[0] == '-')
	{
		i++;
		while (s[i] && s[i] == 'n')
			i++;
		if (!s[i])
			return (1);
	}
	return (0);
}

void	ft_echo(char **av)
{
	int	i;
	int	nl;

	i = 1;
	nl = 1;
	while (av[i] && is_flag(av[i]))
	{
		i++;
		nl = 0;
	}
	while (av[i])
	{
		ft_printf("%s", av[i]);
		i++;
		if (av[i])
			ft_printf(" ");
	}
	if (nl)
		ft_printf("\n");
	free_tab(av);
}
