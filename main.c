#include "inc/minishell.h"

int	main(void)
{
	char		*input;
	t_token		*tokens;
	t_ast_node	*ast;
	t_env		*env;

	// Initialize a simple environment for testing
	env = malloc(sizeof(t_env));
	env->key = ft_strdup("USER");
	env->value = ft_strdup("lowatell");
	env->next = NULL;

	while (1)
	{
		input = readline("minishell> ");
		if (!input)
			break;
		if (*input)
			add_history(input);

		// Lexer
		tokens = lexer(input);
		printf("Tokens:\n");
		for (t_token *tmp = tokens; tmp; tmp = tmp->next)
			printf("Value: %s, Type: %d\n", tmp->value, tmp->type);

		// Expander
		expand_tokens(tokens, env);
		printf("Expanded Tokens:\n");
		for (t_token *tmp = tokens; tmp; tmp = tmp->next)
			printf("Value: %s, Type: %d\n", tmp->value, tmp->type);

		// Parser
		ast = parser(tokens);
		printf("AST Root Token: %s\n", ast ? ast->token->value : "NULL");

		// Free resources
		free_ast(ast);
		free_tokens(tokens);
		free(input);
	}

	// Free environment
	free(env->key);
	free(env->value);
	free(env);
	return (0);
}
