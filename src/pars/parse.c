/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 15:10:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 16:25:42 by lowatell         ###   ########.fr       */
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

t_cmd	*parse_input(char *input, t_data *data)
{
	t_cmd	*cmd_list;
	t_cmd	*new_cmd;
	char	**tokens;
	int		i;

	i = 0;
	cmd_list = NULL;
	(void)data;
	if (!input)
		return (NULL);
	tokens = ft_split(input, ' ');
	if (!tokens)
		return (NULL);
	new_cmd = create_new_cmd();
	if (!new_cmd)
	{
		free_tab(tokens);
		return (NULL);
	}
	new_cmd->args = malloc(sizeof(char *) * (ft_tablen(tokens) + 1)); // Allocate memory for args
	if (!new_cmd->args)
	{
		free_tab(tokens);
		free(new_cmd);
		return (NULL);
	}
	while (tokens[i])
	{
		new_cmd->args[i] = ft_strdup(tokens[i]); // Use new_cmd->args instead of cmd_list->args
		if (!new_cmd->args[i])
		{
			free_tab(tokens);
			free_tab(new_cmd->args); // Free allocated args
			free(new_cmd);
			return (NULL);
		}
		i++;
	}
	new_cmd->args[i] = NULL; // Null-terminate the args array
	add_cmd_to_list(&cmd_list, new_cmd);
	free_tab(tokens); // Free tokens after use
	return (cmd_list);
}
