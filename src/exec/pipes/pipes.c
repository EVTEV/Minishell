/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/07 13:38:41 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"
#include <signal.h>

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
		return ;
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
		return ;
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

/* Gère les signaux pour les processus enfants */
static void	handle_child_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

/* Exécute les commandes avec pipes entre elles */
int	execute_piped_commands(t_data *data)
{
	int		cmd_count;
	int		pipe_count;
	int		result;

	handle_child_signals();
	cmd_count = count_commands(data->cmd_list);
	result = check_cmd_count(data, cmd_count);
	if (result > 0)
		return (result);
	pipe_count = cmd_count - 1;
	if (create_pipes(data, pipe_count) != 0)
		return (1);
	return (execute_pipe_processes(data, cmd_count, pipe_count));
}
