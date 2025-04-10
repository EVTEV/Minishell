/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_processes.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/08 08:09:12 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Configure les pipes pour le processus enfant */
static void	setup_child_pipes(t_data *data, int i, int cmd_count)
{
	if (i > 0)
	{
		if (dup2(data->pipes[i - 1][0], STDIN_FILENO) < 0)
			exit(1);
	}
	if (i < cmd_count - 1)
	{
		if (dup2(data->pipes[i][1], STDOUT_FILENO) < 0)
			exit(1);
	}
	close_all_pipes(data, cmd_count - 1);
}

/* Gère les erreurs de fork et nettoie les ressources */
static int	handle_fork_error(pid_t *pids, int i, t_data *data, int pipe_count)
{
	perror("fork");
	while (--i >= 0)
		waitpid(pids[i], NULL, 0);
	free(pids);
	close_all_pipes(data, pipe_count);
	free_pipes(data, pipe_count); // Libère les pipes en cas d'erreur
	return (1);
}

/* Crée les processus enfants pour chaque commande */
static int	create_child_processes(t_data *data, pid_t *pids,
									int cmd_count, int pipe_count)
{
	t_cmd	*current;
	char	*tmp;
	int		i;

	current = data->cmd_list;
	i = 0;
	while (current && i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] < 0)
			return (handle_fork_error(pids, i, data, pipe_count));
		else if (pids[i] == 0)
		{
			setup_child_pipes(data, i, cmd_count);
			if (setup_redirections(current->redirections) != 0) // Handle redirection errors
			{
				ft_putstr_fd("minishell: error setting up redirections\n", STDERR_FILENO);
				close_all_pipes(data, pipe_count);
				exit(1); // Exit child process with error
			}
			if (!current->args || !current->args[0])
			{
				ft_putstr_fd("minishell: : command not found\n", STDERR_FILENO);
				close_all_pipes(data, pipe_count);
				exit(127);
			}
			tmp = find_command_path(current->args[0], data);
			if (!tmp)
			{
				ft_putstr_fd("minishell: ", STDERR_FILENO);
				if (current->args[0])
					ft_putstr_fd(current->args[0], STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				close_all_pipes(data, pipe_count);
				free_pipes(data, pipe_count); // Libère les pipes en cas d'erreur
				exit(127);
			}
			execute_command_in_child(current, data, tmp);
			close_all_pipes(data, pipe_count);
			exit_clean(data, NULL, 0);
		}
		current = current->next;
		i++;
	}
	return (0);
}

/* Exécute les processus avec pipes et attend leur terminaison */
int	execute_pipe_processes(t_data *data, int cmd_count, int pipe_count)
{
	pid_t	*pids;
	int		exit_status;
	int		i;
	int		status;

	pids = allocate_pids(cmd_count, pipe_count, data);
	if (!pids)
		return (1);
	if (create_child_processes(data, pids, cmd_count, pipe_count) != 0)
		return (1);
	close_all_pipes(data, pipe_count);
	exit_status = 0;
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
		if (WIFEXITED(status) && WEXITSTATUS(status) != 0)
			exit_status = WEXITSTATUS(status);
	}
	free_pids(pids);
	free_pipes(data, pipe_count);
	return (exit_status);
}
