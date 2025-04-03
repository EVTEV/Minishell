/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 00:58:24 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/04 00:58:41 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

void	print_ast(t_ast_node *node)
{
	int	i;

	if (!node)
		return ;
	printf("Command: %s\n", node->args[0]);
	i = 1;
	while (node->args[i])
	{
		printf("Arg %d: %s\n", i, node->args[i]);
		i++;
	}
	print_ast(node->left);
	print_ast(node->right);
}

int	main(int argc, char **argv, char **envp)
{
	char		*input;
	t_token		*tokens;
	t_env		*env;
	t_ast_node	*ast;

	(void)argc;
	(void)argv;
	env = init_env(envp);
	input = readline("minishell> ");
	if (!input)
		return (1);
	tokens = lexer(input);
	expand_tokens(tokens, env);
	ast = parser(tokens);
	print_ast(ast);
	free_tokens(tokens);
	free_ast(ast);
	free_env(env);
	free(input);
	return (0);
}
