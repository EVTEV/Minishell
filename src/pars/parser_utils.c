/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 19:46:05 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/04 00:52:53 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*duplicate_token(t_token *token)
{
	t_token	*new_token;

	if (!token)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	if (token->value)
		new_token->value = ft_strdup(token->value);
	else
		new_token->value = NULL;
	if (!new_token->value)
	{
		free(new_token);
		return (NULL);
	}
	new_token->type = token->type;
	new_token->next = NULL;
	return (new_token);
}

t_ast_node	*create_ast_node(t_token *token)
{
	t_ast_node	*node;

	node = malloc(sizeof(t_ast_node));
	if (!node)
		return (NULL);
	node->token = duplicate_token(token);
	if (!node->token)
	{
		free(node);
		return (NULL);
	}
	node->args = NULL;
	node->left = NULL;
	node->right = NULL;
	return (node);
}

void	remove_right_node(t_ast_node *current)
{
	t_ast_node	*tmp;

	tmp = current->right;
	current->right = tmp->right;
	free(tmp->token->value);
	free(tmp->token);
	free(tmp);
}

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*node;
	int			arg_count;

	if (!*tokens)
		return (NULL);
	node = create_ast_node(*tokens);
	if (!node)
		return (NULL);
	*tokens = (*tokens)->next;
	arg_count = count_arguments(*tokens);
	node->args = fill_args(node, tokens, arg_count);
	if (!node->args)
	{
		free_ast(node);
		return (NULL);
	}
	while (arg_count--)
		*tokens = (*tokens)->next;
	return (node);
}
