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
		data->input = read_input();
		if (data->input && data->input[0]
			&& ft_strncmp(data->input[0], "exit", 4) == 0)
			exit_clean(data, ast);
		if (data->input)
		{
			ast = pars_command(data->input, data);
			if (ast && !ast->type)
				exit_clean(data, ast);
			if (ast && ast->left && ast->left->args)
				print_tab(ast->left->args);
		}
	}
	return (0);
}
