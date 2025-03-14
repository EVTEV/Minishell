#include "inc/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	t_ast	*ast;

	ast = NULL;
	data = init_data(ac, av, env);
	while (1)
	{
		if (ast)
			free_nodes(ast);
		data->input = NULL;
		data->input = read_input(data);
		if (data->input)
		{
			ast = pars_command(data->input, data);
			if (!ast)
				exit_clean(data, ast);
			if (ast && ast->args[0]
				&& ft_strncmp(ast->args[0], "exit", 4) == 0)
				exit_clean(data, ast);
			if (ast && ast->left && ast->left->args)
				print_tab(ast->left->args);
		}
		else
			exit_clean(data, ast);
	}
	return (0);
}
