/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:30:59 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/03 17:42:46 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*node;

	if (!*tokens)
		return (NULL);
	node = create_ast_node(*tokens);
	*tokens = (*tokens)->next;
	return (node);
}

char	*remove_quotes(char *value)
{
	char	*result;
	char	*tmp;
	int		i;
	int		j;
	char	quote;

	result = allocate_result_buffer(value);
	if (!result)
		return (NULL);
	i = -1;
	j = 0;
	quote = 0;
	while (value[++i])
	{
		handle_quote_state(value[i], &quote);
		if (!quote || value[i] != quote)
			result[j++] = value[i];
	}
	result[j] = '\0';
	tmp = ft_strdup(result);
	free(result);
	return (tmp);
}

void	normalize_ast(t_ast_node *root)
{
	t_ast_node	*current;

	if (!root || !root->token)
		return ;
	current = root;
	while (current && current->right)
	{
		if (current->token && current->right->token)
		{
			combine_token_values(current);
			remove_right_node(current);
		}
		else
			current = current->right;
	}
}

t_ast_node	*parser(t_token *tokens)
{
	t_ast_node	*root;

	root = parse_command(&tokens);
	normalize_ast(root);
	return (root);
}
