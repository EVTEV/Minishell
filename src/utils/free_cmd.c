/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:00:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 16:12:09 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Libère la liste des redirections d'une commande */
static void	free_redirections(t_redir *redirections)
{
	t_redir	*current;
	t_redir	*next;

	current = redirections;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}

/* Libère la liste complète des commandes et leurs redirections */
void	free_cmd_list(t_cmd *cmd_list)
{
	t_cmd	*current;
	t_cmd	*next;
	int		i;

	i = 0;
	current = cmd_list;
	while (current)
	{
		next = current->next;
		if (current->args)
		{
			while (current->args[i])
			{
				free(current->args[i]);
				current->args[i] = NULL;
				i++;
			}
		}
		if (current->redirections)
			free_redirections(current->redirections);
		free(current);
		current = next;
	}
}
