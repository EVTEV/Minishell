/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/22 10:48:03 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/22 10:50:52 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_quit(int sig)
{
	if (sig == SIGQUIT)
	{
		g_exit_status = 131;
		ft_putstr_fd("Quit: 3\n", STDOUT_FILENO);
	}
}

void	ft_clean_all(int sig)
{
	t_data	*data;

	data = (t_data *)(long)g_exit_status;
	g_exit_status = 130;
	if (sig == SIGINT)
	{
		free_cmd_list((t_cmd *)(long)data->tmp);
		exit_clean(data, data->tokens, g_exit_status);
	}
}

void	signnn(t_data *data, int i)
{
	data->exit_status = g_exit_status;
	data->tmp = i;
	g_exit_status = (int)(long)data;
	signal(SIGINT, ft_clean_all);
}
