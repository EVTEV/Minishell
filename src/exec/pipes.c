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

#include "../../inc/minishell.h"

static int	count_commands(t_cmd *cmd_list)
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

static int	allocate_pipes(t_data *data, int pipe_count)
{
	int	i;

	data->pipes = (int **)malloc(sizeof(int *) * pipe_count);
	if (!data->pipes)
		return (1);
	
	i = 0;
	while (i < pipe_count)
	{
		data->pipes[i] = (int *)malloc(sizeof(int) * 2);
		if (!data->pipes[i])
		{
			while (--i >= 0)
				free(data->pipes[i]);
			free(data->pipes);
			data->pipes = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

static int	create_pipes(t_data *data, int pipe_count)
{
	int	i;

	if (pipe_count <= 0)
		return (0);
	
	if (allocate_pipes(data, pipe_count) != 0)
		return (1);
	
	i = 0;
	while (i < pipe_count)
	{
		if (pipe(data->pipes[i]) < 0)
		{
			// Cleanup on error
			while (--i >= 0)
			{
				close(data->pipes[i][0]);
				close(data->pipes[i][1]);
				free(data->pipes[i]);
			}
			free(data->pipes);
			data->pipes = NULL;
			return (1);
		}
		i++;
	}
	return (0);
}

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

static void	setup_child_pipes(t_data *data, int i, int cmd_count)
{
	// Set up stdin (input) from the previous pipe
	if (i > 0)
	{
		if (dup2(data->pipes[i - 1][0], STDIN_FILENO) < 0)
			exit(1);
	}
	
	// Set up stdout (output) to the next pipe
	if (i < cmd_count - 1)
	{
		if (dup2(data->pipes[i][1], STDOUT_FILENO) < 0)
			exit(1);
	}
	
	// Close all pipe file descriptors
	close_all_pipes(data, cmd_count - 1);
}

static int	execute_command_in_child(t_cmd *cmd, t_data *data)
{
	char	*cmd_path;
	
	// Apply redirections
	if (setup_redirections(cmd->redirections) != 0)
		exit(1);
	
	// Handle built-in commands
	if (is_builtin(cmd->args[0]))
		exit(execute_builtin(cmd, data));
	
	// Handle external commands
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
	return (0); // Never reached
}

int	execute_piped_commands(t_data *data)
{
	t_cmd	*current;
	int		cmd_count;
	int		pipe_count;
	int		i;
	int		status;
	pid_t	*pids;

	cmd_count = count_commands(data->cmd_list);
	if (cmd_count <= 0)
		return (1);
	
	// If only one command, no need for pipes
	if (cmd_count == 1)
		return (execute_single_command(data->cmd_list, data));
	
	pipe_count = cmd_count - 1;
	if (create_pipes(data, pipe_count) != 0)
		return (1);
	
	// Allocate memory for child process IDs
	pids = (pid_t *)malloc(sizeof(pid_t) * cmd_count);
	if (!pids)
	{
		free_pipes(data, pipe_count);
		return (1);
	}
	
	// Create child processes for each command
	current = data->cmd_list;
	i = 0;
	while (current && i < cmd_count)
	{
		pids[i] = fork();
		if (pids[i] < 0)
		{
			// Handle fork error
			perror("fork");
			while (--i >= 0)
				waitpid(pids[i], NULL, 0);
			free(pids);
			close_all_pipes(data, pipe_count);
			free_pipes(data, pipe_count);
			return (1);
		}
		else if (pids[i] == 0)
		{
			// Child process
			setup_child_pipes(data, i, cmd_count);
			execute_command_in_child(current, data);
		}
		current = current->next;
		i++;
	}
	
	// Parent process: close all pipes and wait for children
	close_all_pipes(data, pipe_count);
	
	// Wait for all child processes
	status = 0;
	for (i = 0; i < cmd_count; i++)
	{
		waitpid(pids[i], &status, 0);
	}
	
	free(pids);
	free_pipes(data, pipe_count);
	
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	
	return (1);
} 