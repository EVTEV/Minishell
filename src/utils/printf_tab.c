/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printf_tab.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:49:18 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 22:15:15 by lowatell         ###   ########.fr       */
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
void	print_cmds(t_cmd *cmds)
{
	int		i;
	t_redir	*redir;

	while (cmds)
	{
		ft_printf("=== Command ===\n");
		i = 0;
		while (cmds->args && cmds->args[i])
		{
			ft_printf("arg[%d]: %s\n", i, cmds->args[i]);
			i++;
		}
		redir = cmds->redir;
		while (redir)
		{
			ft_printf("redir: type = %d, file = %s\n", redir->type, redir->file);
			redir = redir->next;
		}
		cmds = cmds->next;
		if (cmds)
			ft_printf("|\n");
	}
}
