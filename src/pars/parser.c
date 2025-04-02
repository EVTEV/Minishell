#include "../../inc/minishell.h"

t_token	*duplicate_token(t_token *token)
{
	t_token	*new_token;

	if (!token)
		return (NULL);
	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->value = ft_strdup(token->value);
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
	node->token = duplicate_token(token); // Duplicate the token
	if (!node->token)
	{
		free(node);
		return (NULL);
	}
	node->left = NULL;
	node->right = NULL;
	return (node);
}

t_ast_node	*parse_command(t_token **tokens)
{
	t_ast_node	*node;

	if (!*tokens)
		return (NULL);
	node = create_ast_node(*tokens);
	*tokens = (*tokens)->next;
	return (node);
}

t_ast_node	*parser(t_token *tokens)
{
	t_ast_node	*root;

	root = parse_command(&tokens);
	return (root);
}

void	free_ast(t_ast_node *root)
{
	if (!root)
		return ;
	free_ast(root->left);
	free_ast(root->right);
	if (root->token)
	{
		if (root->token->value)
		{
			free(root->token->value);
			root->token->value = NULL; // Set to NULL after freeing
		}
		free(root->token);
		root->token = NULL; // Set to NULL after freeing
	}
	free(root);
	root = NULL; // Set to NULL after freeing
}
