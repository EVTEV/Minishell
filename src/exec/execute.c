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

/* Vérifie si une commande est une commande interne */
int	is_builtin(char *cmd)
{
	if (!cmd)
		return (0);
	if (ft_strncmp(cmd, "echo", 5) == 0)
		return (1);
	if (ft_strncmp(cmd, "cd", 3) == 0)
		return (1);
	if (ft_strncmp(cmd, "pwd", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "export", 7) == 0)
		return (1);
	if (ft_strncmp(cmd, "unset", 6) == 0)
		return (1);
	if (ft_strncmp(cmd, "env", 4) == 0)
		return (1);
	if (ft_strncmp(cmd, "exit", 5) == 0)
		return (1);
	return (0);
}

/* Calcule la longueur d'un tableau de chaînes */
static int	ft_tablen(char **tab)
{
	int	i;

	i = 0;
	while (tab && tab[i])
		i++;
	return (i);
}

/* Exécute une commande interne */
int	execute_builtin(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	if (ft_strncmp(cmd->args[0], "echo", 5) == 0)
		return (ft_echo(ft_tablen(cmd->args), cmd->args));
	else if (ft_strncmp(cmd->args[0], "cd", 3) == 0)
		return (ft_cd(cmd->args, data->env_list));
	else if (ft_strncmp(cmd->args[0], "pwd", 4) == 0)
		return (ft_pwd());
	else if (ft_strncmp(cmd->args[0], "export", 7) == 0)
		return (ft_export(cmd->args, &data->env_list));
	else if (ft_strncmp(cmd->args[0], "unset", 6) == 0)
		return (ft_unset(cmd->args, &data->env_list));
	else if (ft_strncmp(cmd->args[0], "env", 4) == 0)
		return (print_tab(data->env), 0);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
		exit_clean(data);
	return (1);
}

/* Exécute une commande externe dans un processus enfant */
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

/* Exécute une commande unique (interne ou externe) */
int	execute_single_command(t_cmd *cmd, t_data *data)
{
	if (!cmd || !cmd->args || !cmd->args[0])
		return (1);
	
	if (is_builtin(cmd->args[0]))
		return (execute_builtin(cmd, data));
	else
		return (execute_external(cmd, data));
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