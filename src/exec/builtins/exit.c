/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 10:56:28 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Libère tous les membres de la structure data */
static void	free_data_members(t_data *data)
{
	if (data->input)
		free(data->input);
	if (data->env)
		free_tab(data->env); // Ensure free_tab handles NULL pointers
	if (data->env_list)
		free_env_copy(data->env_list);
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
