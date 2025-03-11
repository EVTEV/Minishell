#include "inc/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	*data;
	t_ast	*ast;

	data = init_data(ac, av, env);
	while (1)
	{
		data->input = NULL;
		ft_printf("minishell: ");
		data->input = read_input(data);
		if (data->input)
		{
			ast = pars_command(data->input);
			print_nodes(ast);
			//free_tab(data->input);
		}
	}
	return (0);
}
