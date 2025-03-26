/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 22:08:46 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 22:12:28 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_redir	*create_redir(char *file, t_token_type type)
{
	t_redir	*redir;

	redir = malloc(sizeof(t_redir));
	if (!redir)
		return (NULL);
	redir->file = file;
	redir->type = type;
	redir->next = NULL;
	return (redir);
}

t_cmd	*create_cmd(t_token **tok, t_env *env, int status)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = extract_args(tok, env, status);
	if (!cmd->args)
		return (free(cmd), NULL);
	cmd->redir = extract_redirs(tok);
	cmd->next = NULL;
	if (*tok && (*tok)->type == PIPE)
		*tok = (*tok)->next;
	return (cmd);
}

t_cmd	*parse_tokens(t_token *tokens, t_env *env, int status)
{
	t_cmd	*head;
	t_cmd	*cur;
	t_cmd	*new;

	head = NULL;
	while (tokens)
	{
		new = create_cmd(&tokens, env, status);
		if (!new)
			return (free_cmds(head), NULL);
		if (!head)
			head = new;
		else
			cur->next = new;
		cur = new;
	}
	return (head);
}

void	free_cmds(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		i;
	t_redir	*r_tmp;

	while (cmd)
	{
		tmp = cmd->next;
		i = 0;
		while (cmd->args && cmd->args[i])
			free(cmd->args[i++]);
		free(cmd->args);
		while (cmd->redir)
		{
			r_tmp = cmd->redir->next;
			free(cmd->redir->file);
			free(cmd->redir);
			cmd->redir = r_tmp;
		}
		free(cmd);
		cmd = tmp;
	}
}
