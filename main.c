#include "inc/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	data = init_data(ac, av, env);
	while (1)
	{
		ft_printf("minishell: ");
		if (read_input(data) && data->input)
		{
			data->cmd_list = parse_input(data->input, data);
			if (data->cmd_list)
			{
				data->exit_status = execute_commands(data);
				free_cmd_list(data->cmd_list);
				data->cmd_list = NULL;
			}
			free(data->input);
			data->input = NULL;
		}
	}
	return (0);
}
