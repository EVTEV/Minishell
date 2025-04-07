#include "inc/minishell.h"
#include <signal.h>

static void	setup_signals(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	setup_signals();
	data = init_data(ac, av, env);
	data->cmd_list = NULL;
	while (1)
	{
		data->input = read_input();
		if (data->input)
		{
			char *expanded_input = expander(data->input, data); // Passez `data` ici
			t_token *tokens = lexer(expanded_input);
			free(expanded_input);
			data->cmd_list = parser(tokens);
			if (data->cmd_list)
			{
				data->exit_status = execute_commands(data);
				if (data->cmd_list)
				{
					free_cmd_list(data->cmd_list);
					data->cmd_list = NULL;
				}
			}
			// Libérer les tokens après parsing
			free(data->input);
			data->input = NULL;
		}
	}
	return (0);
}
