/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 16:04:28 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Vérifie les arguments de la commande echo pour l'option -n */
static int	check_args(char **args, int *start)
{
	int	i;
	int	j;

	i = 1;
	while (args[i] && args[i][0] == '-')
	{
		j = 0;
		while (args[i][j] && args[i][j] == 'n')
			j++;
		i++;
	}
	*start = i;
	if (i > 1)
		return (1);
	return (0);
}

/* Écrit les arguments de la commande echo avec ou sans retour à la ligne */
static void	write_args(char **args, int start, int newline)
{
	int	i;

	i = start;
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
}

/* Fonction principale de la commande echo */
int	ft_echo(int ac, char **av)
{
	int	newline;
	int	start;

	if (ac < 2)
	{
		ft_printf("\n");
		return (0);
	}
	newline = !check_args(av, &start);
	write_args(av, start, newline);
	return (0);
}
