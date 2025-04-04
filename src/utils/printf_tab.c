/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:49:18 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/04 13:44:46 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Affiche le contenu d'un tableau de chaînes de caractères */
void	print_tab(char **tab)
{
	int	i;

	if (!tab || !*tab)
		return ;
	i = -1;
	while (tab[++i])
		ft_printf("%s\n", tab[i]);
}

void	print_list(t_env *env)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (current->value)
			ft_printf("%s=%s\n", current->name, current->value);
		else
			ft_printf("%s\n", current->name);
		current = current->next;
	}
}
