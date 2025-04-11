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

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return (NULL);
	}
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
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		return ;
	}
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

static int	handle_word_token(t_cmd *cmd, t_token *token, t_cmd *cmd_list)
{
	char	*dup_value;

	dup_value = ft_strdup(token->value);
	if (!dup_value)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		free_cmd_list(cmd_list);
		return (0);
	}
	cmd->args = ft_tabjoin(cmd->args, dup_value);
	if (!cmd->args)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		free(dup_value);
		free_cmd_list(cmd_list);
		return (0);
	}
	return (1);
}

static int	handle_pipe_token(t_cmd **current_cmd, t_cmd *cmd_list)
{
	if (!*current_cmd || !(*current_cmd)->args)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `|'\n",
			STDERR_FILENO);
		free_cmd_list(cmd_list);
		return (0);
	}
	(*current_cmd)->next = create_new_command();
	if (!(*current_cmd)->next)
	{
		free_cmd_list(cmd_list);
		return (0);
	}
	*current_cmd = (*current_cmd)->next;
	return (1);
}

static int	handle_redirection_file(t_cmd *current_cmd, t_token **tokens,
	t_cmd *cmd_list)
{
	char	*file;

	file = ft_strdup((*tokens)->next->value);
	if (!file)
	{
		ft_putstr_fd("minishell: memory allocation error\n", STDERR_FILENO);
		free_cmd_list(cmd_list);
		return (0);
	}
	add_redirection(&current_cmd->redirections, (*tokens)->type, file);
	if (!current_cmd->redirections)
	{
		free(file);
		free_cmd_list(cmd_list);
		return (0);
	}
	*tokens = (*tokens)->next;
	return (1);
}

static int	handle_redirection_token(t_cmd *current_cmd, t_token **tokens,
	t_cmd *cmd_list)
{
	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
		free_cmd_list(cmd_list);
		return (0);
	}
	return (handle_redirection_file(current_cmd, tokens, cmd_list));
}

static int	handle_heredoc_token(t_cmd *current_cmd, t_token **tokens,
	t_cmd *cmd_list)
{
	char	*heredoc_file;

	if (!(*tokens)->next || (*tokens)->next->type != TOKEN_WORD)
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
		free_cmd_list(cmd_list);
		return (0);
	}
	if (handle_heredoc((*tokens)->next->value, &heredoc_file) != 0)
	{
		free_cmd_list(cmd_list);
		return (0);
	}
	add_redirection(&current_cmd->redirections, TOKEN_REDIR_IN, heredoc_file);
	if (!current_cmd->redirections)
	{
		free(heredoc_file);
		free_cmd_list(cmd_list);
		return (0);
	}
	*tokens = (*tokens)->next;
	return (1);
}

static t_cmd	*initialize_command(t_cmd **current_cmd, t_cmd *cmd_list)
{
	*current_cmd = create_new_command();
	if (!*current_cmd)
	{
		free_cmd_list(cmd_list);
		return (NULL);
	}
	if (!cmd_list)
		cmd_list = *current_cmd;
	return (cmd_list);
}

static int	process_token(t_cmd **current_cmd, t_cmd **cmd_list,
	t_token **tokens)
{
	if ((*tokens)->type == TOKEN_WORD)
	{
		if (!handle_word_token(*current_cmd, *tokens, *cmd_list))
			return (0);
	}
	else if ((*tokens)->type == TOKEN_PIPE)
	{
		if (!handle_pipe_token(current_cmd, *cmd_list))
			return (0);
	}
	else if ((*tokens)->type == TOKEN_REDIR_OUT
		|| (*tokens)->type == TOKEN_REDIR_APPEND
		|| (*tokens)->type == TOKEN_REDIR_IN)
	{
		if (!handle_redirection_token(*current_cmd, tokens, *cmd_list))
			return (0);
	}
	else if ((*tokens)->type == TOKEN_REDIR_HEREDOC)
	{
		if (!handle_heredoc_token(*current_cmd, tokens, *cmd_list))
			return (0);
	}
	return (1);
}

static int	finalize_command(t_cmd *current_cmd, t_cmd *cmd_list)
{
	if (current_cmd && !current_cmd->args && !current_cmd->redirections)
	{
		free_cmd_list(cmd_list);
		return (0);
	}
	return (1);
}

t_cmd	*parser(t_token *tokens)
{
	t_cmd	*cmd_list;
	t_cmd	*current_cmd;

	cmd_list = NULL;
	current_cmd = NULL;
	while (tokens)
	{
		if (!current_cmd)
		{
			cmd_list = initialize_command(&current_cmd, cmd_list);
			if (!cmd_list)
				return (NULL);
		}
		if (!process_token(&current_cmd, &cmd_list, &tokens))
			return (NULL);
		tokens = tokens->next;
	}
	if (!finalize_command(current_cmd, cmd_list))
		return (NULL);
	return (cmd_list);
}
