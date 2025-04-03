/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:30:59 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/04 00:54:23 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	count_arguments(t_token *tokens)
{
	int	count;

	count = 0;
	while (tokens && tokens->type == WORD)
	{
		count++;
		tokens = tokens->next;
	}
	return (count);
}

char	**fill_args(t_ast_node *node, t_token **tokens, int arg_count)
{
	char	**args;
	t_token	*current_token;
	int		i;

	args = malloc((arg_count + 2) * sizeof(char *));
	if (!args)
		return (NULL);
	args[0] = ft_strdup(node->token->value);
	if (!args[0])
	{
		free(args);
		return (NULL);
	}
	current_token = *tokens;
	i = 1;
	while (i <= arg_count)
	{
		args[i] = ft_strdup(current_token->value);
		current_token = current_token->next;
		i++;
	}
	args[arg_count + 1] = NULL;
	return (args);
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
