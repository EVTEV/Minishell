#include "inc/minishell.h"

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
			setup_exec_signals();
			char *expanded_input = expander(data->input, data);
			free(data->input);
			data->input = NULL;
			t_token *tokens = lexer(expanded_input);
			if (!tokens)
			{
				free(expanded_input);
				continue;
			}
			free(expanded_input);
			data->cmd_list = parser(tokens);
			if (!data->cmd_list) // Erreur de syntaxe ou interruption
			{
				if (data->input)
				{
					free(data->input);
					data->input = NULL;
				}
				free_token(tokens);
				tokens = NULL;
				data->exit_status = 2; // Code de retour pour erreur de syntaxe
				continue;
			}
			if (data->cmd_list && data->cmd_list->interrupted) // Si le heredoc a été interrompu
			{
				free_cmd_list(data->cmd_list);
				data->cmd_list = NULL;
				free(data->input);
				data->input = NULL;
				free_token(tokens);
				tokens = NULL;
				data->exit_status = 1; // Code de retour pour interruption
				continue;
			}
			if (tokens)
			{
				free_token(tokens);
				tokens = NULL;
			}
			if (data->cmd_list)
			{
				data->exit_status = execute_commands(data);
				if (data && data->cmd_list)
				{
					free_cmd_list(data->cmd_list); // Free the command list after execution
					data->cmd_list = NULL;
				}
			}
			else
				free_cmd_list(data->cmd_list); // Free the command list if execution fails
			setup_signals();
			free(data->input);
			data->input = NULL;
		}
	}
	return (0);
}
