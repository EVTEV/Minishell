/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 13:00:26 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/17 13:33:08 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "inc/minishell.h"

int	g_exit_status = 0;

static int	process_input(t_data *data)
{
	char	*expanded_input;
	t_token	*tokens;

	setup_exec_signals();
	expanded_input = expander(data->input, data);
	if (!expanded_input)
		return (0);
	free(data->input);
	data->input = NULL;
	tokens = lexer(expanded_input);
	free(expanded_input);
	if (!tokens)
		return (0);
	data->cmd_list = parser(tokens);
	free_token(tokens);
	if (!data->cmd_list || (data->cmd_list && data->cmd_list->interrupted))
	{
		if (data->cmd_list)
			data->exit_status = 1;
		else
			data->exit_status = 2;
		return (free_cmd_list(data->cmd_list), data->cmd_list = NULL, 0);
	}
	return (1);
}

static void	execute_and_cleanup(t_data *data)
{
	data->exit_status = execute_commands(data);
	free_cmd_list(data->cmd_list);
	data->cmd_list = NULL;
}


static void	handle_input(t_data *data)
{
	data->input = read_input(data);
	if (!data->input || ft_isspace(data->input))
	{
		free(data->input);
		data->input = NULL;
		return ;
	}
	if (process_input(data))
		execute_and_cleanup(data);
}

static void	main_loop(t_data *data)
{
	while (1)
	{
		handle_input(data);
		setup_signals();
	}
}

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	if (!env || !*env)
	{
		ft_putstr_fd("minishell: env not found\n", 2);
		return (1);
	}
	setup_signals();
	load_history();
	data = init_data(ac, av, env);
	if (!data)
	{
		free_data_members(data);
		free(data);
		data = NULL;
		perror("minishell: init_data");
		return (1);
	}
	data->cmd_list = NULL;
	main_loop(data);
	return (0);
}
