/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:24:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 10:18:26 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Calcule la longueur d'un tableau de chaînes */
int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

/* Gère l'exécution dans le processus enfant */
static void	execute_child_process(t_cmd *cmd, t_data *data, char *cmd_path)
{
	if (setup_redirections(cmd->redirections) != 0)
		exit(1);
	if (execve(cmd_path, cmd->args, data->env) == -1)
	{
		ft_printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
		exit(126);
	}
}

/* Attend la fin du processus enfant et retourne son statut */
static int	wait_for_child(pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

/* Exécute une commande externe dans un processus enfant */
int	execute_external(t_cmd *cmd, t_data *data)
{
	char	*cmd_path;
	pid_t	pid;

	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	cmd_path = find_command_path(cmd->args[0], data);
	if (!cmd_path)
	{
		ft_printf("minishell: %s: command not found\n", cmd->args[0]);
		return (127);
	}
	pid = fork();
	if (pid == 0)
		execute_child_process(cmd, data, cmd_path);
	free(cmd_path);
	return (wait_for_child(pid));
}

/* Exécute la liste des commandes avec ou sans pipes */
int	execute_commands(t_data *data)
{
	if (!data || !data->cmd_list)
		return (1);
	if (!data->cmd_list->next)
		return (execute_single_command(data->cmd_list, data));
	return (execute_piped_commands(data));
}
