#include "inc/minishell.h"
#include <signal.h>

static void	setup_signals(void)
{
	signal(SIGINT, signal_handler_main);
	signal(SIGQUIT, SIG_IGN);
}

static void	setup_exec_signals(void)
{
	signal(SIGINT, ft_exec_sig_handler);
	signal(SIGQUIT, ft_exec_sig_handler);
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	setup_signals();
	load_history();
	data = init_data(ac, av, env);
	data->cmd_list = NULL;
	while (1)
	{
		data->input = read_input(data);
		if (data->input)
		{
			setup_exec_signals(); // Configure les signaux pour l'exécution
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
			setup_signals(); // Réinitialise les signaux pour le prompt
			free(data->input);
			data->input = NULL;
		}
	}
	return (0);
}
