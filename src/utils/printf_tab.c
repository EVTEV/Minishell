/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:49:18 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/11 18:18:03 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_tab(char **tab)
{
	int	i;

	if (!tab || !*tab)
		return ;
	i = -1;
	while (tab[++i])
	{
		ft_printf("%s", tab[i]);
		ft_printf("\n");
	}
}
