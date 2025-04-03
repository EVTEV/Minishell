/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_processes.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 14:50:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Exécute une commande dans un processus enfant */
int	execute_command_in_child(t_cmd *cmd, t_data *data)
{
	char	*cmd_path;
	
	if (setup_redirections(cmd->redirections) != 0)
		exit(1);
	
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, data));
	
	cmd_path = find_command_path(cmd->args[0], data);
	if (!cmd_path)
	{
		ft_printf("minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	
	if (execve(cmd_path, cmd->args, data->env) < 0)
	{
		free(cmd_path);
		ft_printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
		exit(126);
	}
	return (0);
}

/* Attend la terminaison de tous les processus enfants et retourne le statut de sortie */
int	wait_for_children(pid_t *pids, int cmd_count)
{
	int	i;
	int	status;
	int	exit_status;

	i = 0;
	status = 0;
	while (i < cmd_count)
	{
		waitpid(pids[i], &status, 0);
		i++;
	}
	free(pids);
	if (WIFEXITED(status))
		exit_status = WEXITSTATUS(status);
	else
		exit_status = 1;
	return (exit_status);
} 