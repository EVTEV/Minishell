/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:00:26 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/11 20:00:11 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	g_exit_status = 0;

/* Gère SIGINT dans le parent pour ne pas quitter le shell */
void	signal_handler_main(int signum)
{
	if (signum == SIGINT)
	{
		ft_putchar_fd('\n', STDOUT_FILENO);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		g_exit_status = 130; // Code de sortie pour SIGINT
	}
}

void	setup_signals(void)
{
	signal(SIGINT, signal_handler_main); // Gérer SIGINT dans le parent
	signal(SIGQUIT, SIG_IGN); // Ignorer SIGQUIT dans le parent
}

static void	setup_exec_signals(void)
{
	signal(SIGINT, ft_exec_sig_handler);
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
			if (ft_isspace(data->input))
			{
				free(data->input);
				data->input = NULL;
				continue;
			}
			setup_exec_signals();
			char *expanded_input = expander(data->input, data);
			free(data->input);
			data->input = NULL;
			t_token *tokens = lexer(expanded_input);
			if (!tokens)
			{
				free(expanded_input);
				expanded_input = NULL;
				continue ;
			}
			free(expanded_input);
			expanded_input = NULL;
			data->cmd_list = parser(tokens);
			if (!data->cmd_list)
			{
				if (data->input)
				{
					free(data->input);
					data->input = NULL;
				}
				free_token(tokens);
				tokens = NULL;
				data->exit_status = 2;
				continue;
			}
			if (data->cmd_list && data->cmd_list->interrupted)
			{
				free_cmd_list(data->cmd_list);
				data->cmd_list = NULL;
				free(data->input);
				data->input = NULL;
				free_token(tokens);
				tokens = NULL;
				data->exit_status = 1;
				continue ;
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
					free_cmd_list(data->cmd_list);
					data->cmd_list = NULL;
				}
			}
			else
				free_cmd_list(data->cmd_list);
			setup_signals();
			free(data->input);
			data->input = NULL;
		}
	}
	return (0);
}
