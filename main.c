#include "inc/minishell.h"

void	print_ast(t_ast_node *root)
{
	if (!root)
		return;
	if (root->token && root->token->value)
		printf("AST Token: %s\n", root->token->value);
	print_ast(root->left);
	print_ast(root->right);
}

// Fonction pour initialiser un environnement minimal


// Fonction pour libérer l'environnement


// Fonction pour exécuter une commande
void	execute_command(t_ast_node *ast)
{
	if (!ast || !ast->token || !ast->token->value)
		return ;
	if (access(ast->token->value, F_OK) == -1) // Vérifie si la commande existe
	{
		printf("minishell: %s: command not found\n", ast->token->value);
		return ;
	}
	// Exécution de la commande (à implémenter)
}

int	main(int argc, char **argv, char **envp)
{
	t_token		*tokens;
	t_ast_node	*ast;
	t_env		*env;
	char		*input;

	(void)argv;
	(void)argc;
	tokens = NULL;
	ast = NULL;
	env = NULL;
	input = NULL;
	// Initialisation de l'environnement
	env = init_env(envp);
	if (!env)
	{
		printf("Failed to initialize environment.\n");
		return (1);
	}
	// Boucle interactive
	while (1)
	{
		input = readline("minishell> ");
		if (!input) // Gestion de Ctrl+D
		{
			printf("Exiting minishell.\n");
			break;
		}
		if (*input) // Ajouter à l'historique si la commande n'est pas vide
			add_history(input);
		if (ft_strncmp(input, "exit", ft_strlen(input)) == 0)
			break;
		// Lexer : création des tokens
		tokens = lexer(input);
		printf("Tokens:\n");
		for (t_token *tmp = tokens; tmp; tmp = tmp->next)
			printf("Value: %s, Type: %d\n", tmp->value, tmp->type);

		// Expander : expansion des tokens
		expand_tokens(tokens, env);
		printf("\nExpanded Tokens:\n");
		for (t_token *tmp = tokens; tmp; tmp = tmp->next)
			printf("Value: %s, Type: %d\n", tmp->value, tmp->type);

		// Parser : création de l'AST
		ast = parser(tokens);
		printf("\nAST After Normalization:\n");
		print_ast(ast);

		// Exécution de la commande
		execute_command(ast);

		// Cleanup
		free_tokens(tokens);
		free_ast(ast);
		free(input);
	}
	// Libération de l'environnement
	if (tokens)
		free_tokens(tokens);
	if (ast)
		free_ast(ast);
	if (input)
		free(input);
	if (env)
		free_env(env);
	printf("Exiting minishell.\n");
	return (0);
}

