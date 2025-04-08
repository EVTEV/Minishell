/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 16:42:57 by lowatell          #+#             */
/*   Updated: 2025/04/04 18:02:00 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_cmd	*create_new_command(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	ft_memset(cmd, 0, sizeof(t_cmd));
	cmd->next = NULL;
	return (cmd);
}

static void	add_redirection(t_redir **redirections, int type, char *file)
{
	t_redir	*new;
	t_redir	*tmp;

	new = malloc(sizeof(t_redir));
	if (!new)
		return ;
	new->type = type;
	new->file = file;
	new->next = NULL;
	if (!*redirections)
		*redirections = new;
	else
	{
		tmp = *redirections;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmd_list = NULL;
	t_cmd	*current_cmd = NULL;

	while (tokens)
	{
		if (!current_cmd)
		{
			current_cmd = create_new_command();
			if (!current_cmd)
				return (NULL);
			if (!cmd_list)
				cmd_list = current_cmd;
		}
		if (tokens->type == TOKEN_WORD)
		{
			char *dup_value = ft_strdup(tokens->value); // Duplicate token value
			if (!dup_value || !(current_cmd->args = ft_tabjoin(current_cmd->args, dup_value)))
			{
				free(dup_value); // Free if ft_tabjoin fails
				free_cmd_list(cmd_list);
				return (NULL);
			}
		}
		else if (tokens->type == TOKEN_PIPE)
		{
			current_cmd->next = create_new_command();
			current_cmd = current_cmd->next;
		}
		else if (tokens->type == TOKEN_REDIR_OUT || tokens->type == TOKEN_REDIR_APPEND || tokens->type == TOKEN_REDIR_IN)
		{
			if (tokens->next && tokens->next->type == TOKEN_WORD)
			{
				add_redirection(&current_cmd->redirections, tokens->type, ft_strdup(tokens->next->value));
				tokens = tokens->next;
			}
		}
		tokens = tokens->next;
	}
	return (cmd_list);
}
