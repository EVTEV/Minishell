#include "inc/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	t_ast	*ast;

	data = init_data(ac, av, env);
	while (1)
	{
		data->input = NULL;
		data->input = read_input(data);
		if (data->input)
		{
			ast = pars_command(data->input);
			if (ast && ast->args[0]
				&& ft_strncmp(ast->args[0], "exit", 4) == 0)
				exit_clean(data, &ast);
			if (ast && ast->left && ast->left->args)
				print_tab(ast->left->args);
			//execution(ast);
		}
	}
	clear_history();
	free_nodes(ast);
	return (0);
}
