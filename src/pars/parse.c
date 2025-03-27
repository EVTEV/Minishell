/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:10:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 15:10:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_cmd	*create_new_cmd(void)
{
	t_cmd	*new_cmd;

	new_cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!new_cmd)
		return (NULL);
	new_cmd->args = NULL;
	new_cmd->redirections = NULL;
	new_cmd->pipe_in = 0;
	new_cmd->pipe_out = 0;
	new_cmd->next = NULL;
	return (new_cmd);
}

static void	add_cmd_to_list(t_cmd **cmd_list, t_cmd *new_cmd)
{
	t_cmd	*current;

	if (!*cmd_list)
	{
		*cmd_list = new_cmd;
		return ;
	}
	current = *cmd_list;
	while (current->next)
		current = current->next;
	current->next = new_cmd;
}

// Pour l'instant, elle se contente de créer une commande simple à partir de l'entrée
t_cmd	*parse_input(char *input, t_data *data)
{
	t_cmd	*cmd_list;
	t_cmd	*new_cmd;
	char	**tokens;

	(void)data;
	if (!input)
		return (NULL);
	tokens = ft_split(input, ' ');
	if (!tokens)
		return (NULL);
	cmd_list = NULL;
	new_cmd = create_new_cmd();
	if (!new_cmd)
	{
		free_tab(tokens);
		return (NULL);
	}
	new_cmd->args = tokens;
	add_cmd_to_list(&cmd_list, new_cmd);
	return (cmd_list);
} 