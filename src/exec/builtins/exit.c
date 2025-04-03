/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 14:50:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Libère tous les membres de la structure data */
static void	free_data_members(t_data *data)
{
	if (data->input)
		free(data->input);
	if (data->env)
		free_tab(data->env);
	if (data->path)
		free(data->path);
	if (data->cmd_list)
		free_cmd_list(data->cmd_list);
}

/* Nettoie et libère toutes les ressources avant de quitter le programme */
void	exit_clean(t_data *data)
{
	if (!data)
		exit(2);
	free_data_members(data);
	if (data->pipes)
		free_pipes(data, count_commands(data->cmd_list) - 1);
	free(data);
	exit(2);
}