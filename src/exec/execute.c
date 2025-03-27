/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:24:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 14:24:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strcmp(cmd, "echo") == 0)
		return (1);
	if (ft_strcmp(cmd, "cd") == 0)
		return (1);
	if (ft_strcmp(cmd, "pwd") == 0)
		return (1);
	if (ft_strcmp(cmd, "export") == 0)
		return (1);
	if (ft_strcmp(cmd, "unset") == 0)
		return (1);
	if (ft_strcmp(cmd, "env") == 0)
		return (1);
	if (ft_strcmp(cmd, "exit") == 0)
		return (1);
	return (0);
}

int	execute_builtin(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strcmp(cmd->args[0], "echo") == 0)
		return (ft_echo(ft_tablen(cmd->args), cmd->args));
	else if (ft_strcmp(cmd->args[0], "cd") == 0)
		return (ft_cd(cmd->args, NULL)); // Remplacer NULL par l'environnement
	else if (ft_strcmp(cmd->args[0], "exit") == 0)
		exit_clean(data);
	else if (ft_strcmp(cmd->args[0], "env") == 0)
		return (print_tab(data->env), 0);
	// Ajouter les autres commandes internes (pwd, export, unset)
	return (1);
}

char	*find_command_path(char *cmd, t_data *data)
{
	char	**paths;
	char	*tmp;
	char	*cmd_path;
	int		i;

	if (!cmd || !data || !data->path)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	paths = ft_split(data->path + 5, ':'); // Skip "PATH="
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(cmd_path, X_OK) == 0)
			return (free_tab(paths), cmd_path);
		free(cmd_path);
		i++;
	}
	return (free_tab(paths), NULL);
}

int	execute_external(t_cmd *cmd, t_data *data)
{
	char	*cmd_path;
	pid_t	pid;
	int		status;

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
	{
		if (setup_redirections(cmd->redirections) != 0)
			exit(1);
		if (execve(cmd_path, cmd->args, data->env) == -1)
		{
			ft_printf("minishell: %s: %s\n", cmd->args[0], strerror(errno));
			exit(126);
		}
	}
	free(cmd_path);
	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (1);
}

int	execute_single_command(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(cmd, data));
	else
		return (execute_external(cmd, data));
}

int	execute_commands(t_data *data)
{
	if (!data || !data->cmd_list)
		return (1);
	
	// Si une seule commande, pas besoin de pipes
	if (!data->cmd_list->next)
		return (execute_single_command(data->cmd_list, data));
	
	// Sinon, utiliser la fonction qui gère les pipes
	return (execute_piped_commands(data));
} 