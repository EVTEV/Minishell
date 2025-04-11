/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:24:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/11 10:26:23 by lowatell         ###   ########.fr       */
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
		exit_clean(data, NULL, 1);
	if (execve(cmd_path, cmd->args, data->env) == -1)
	{
		ft_printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
		exit_clean(data, NULL, 126);
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
	{
		if (setup_redirections(cmd->redirections) != 0) // Configure les redirections même si la commande est vide
		{
			//ft_putstr_fd("minishell: error setting up redirections\n", STDERR_FILENO);
			return (1);
		}
		return (1); // Retourne 0 pour indiquer que les redirections ont été configurées
	}
	if (setup_redirections(cmd->redirections) != 0) // Configure les redirections avant de vérifier la commande
	{
		//ft_putstr_fd("minishell: error setting up redirections\n", STDERR_FILENO);
		return (1); // Return error for this command but allow pipeline to continue
	}
	cmd_path = find_command_path(cmd->args[0], data);
	if (!cmd_path)
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		return (127); // Retourne une erreur mais les redirections sont déjà configurées
	}
	if (is_directory(cmd_path)) // Check if the command is a directory
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd(": is a directory\n", STDERR_FILENO);
		free(cmd_path);
		return (126); // Return error code for "Is a directory"
	}
	pid = fork();
	if (pid == 0)
		execute_child_process(cmd, data, cmd_path);
	free(cmd_path);
	cmd_path = NULL;
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
