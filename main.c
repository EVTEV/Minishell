#include "inc/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	data = init_data(ac, av, env);
	data->cmd_list = NULL;
	while (1)
	{
		data->input = read_input();
		if (data->input)
		{
			data->cmd_list = parse_input(data->input, data);
			if (data->cmd_list)
			{
				data->exit_status = execute_commands(data);
				if (data->cmd_list)
				{
					free_cmd_list(data->cmd_list);
					data->cmd_list = NULL;
				}
			}
			free(data->input);
			data->input = NULL;
		}
	}
	return (0);
}
