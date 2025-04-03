/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 14:50:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Compte le nombre total de commandes dans la liste */
int	count_commands(t_cmd *cmd_list)
{
	int		count;
	t_cmd	*current;

	count = 0;
	current = cmd_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/* Ferme tous les descripteurs de fichiers des pipes */
void	close_all_pipes(t_data *data, int pipe_count)
{
	int	i;

	if (!data->pipes)
		return;
	
	i = 0;
	while (i < pipe_count)
	{
		close(data->pipes[i][0]);
		close(data->pipes[i][1]);
		i++;
	}
}

/* Libère la mémoire allouée pour les pipes */
void	free_pipes(t_data *data, int pipe_count)
{
	int	i;

	if (!data->pipes)
		return;
	
	i = 0;
	while (i < pipe_count)
	{
		free(data->pipes[i]);
		i++;
	}
	free(data->pipes);
	data->pipes = NULL;
}

/* Vérifie le nombre de commandes et exécute la commande unique si nécessaire */
static int	check_cmd_count(t_data *data, int cmd_count)
{
	if (cmd_count <= 0)
		return (1);
	if (cmd_count == 1)
		return (execute_single_command(data->cmd_list, data));
	return (0);
}

/* Exécute les commandes avec pipes entre elles */
int	execute_piped_commands(t_data *data)
{
	int		cmd_count;
	int		pipe_count;
	int		result;
	
	cmd_count = count_commands(data->cmd_list);
	result = check_cmd_count(data, cmd_count);
	if (result > 0)
		return (result);
		
	pipe_count = cmd_count - 1;
	if (create_pipes(data, pipe_count) != 0)
		return (1);
		
	return (execute_pipe_processes(data, cmd_count, pipe_count));
} 