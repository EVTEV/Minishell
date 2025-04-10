/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:17:23 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/08 08:09:16 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Sauvegarde les descripteurs de fichiers standard */
static int	save_std_fds(int std_fds[2])
{
	std_fds[0] = dup(STDIN_FILENO);
	std_fds[1] = dup(STDOUT_FILENO);
	if (std_fds[0] == -1 || std_fds[1] == -1)
	{
		perror("dup");
		return (1);
	}
	return (0);
}

/* Restaure les descripteurs de fichiers standard */
static void	restore_std_fds(int std_fds[2])
{
	dup2(std_fds[0], STDIN_FILENO);
	dup2(std_fds[1], STDOUT_FILENO);
	close(std_fds[0]);
	close(std_fds[1]);
}

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
		return (print_list(data->env_list), 0);
	else if (ft_strncmp(cmd->args[0], "exit", 5) == 0)
	{
		ft_exit(cmd->args, data); // Pass arguments to the exit function
		return (1); // This line won't be reached as ft_exit exits the program
	}
	else
	{
		ft_putstr_fd("minishell: ", STDERR_FILENO);
		if (cmd->args[0])
		{
			ft_putstr_fd(cmd->args[0], STDERR_FILENO);
			ft_putstr_fd("\n", STDERR_FILENO);
		}
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
	}
	return (0);
}

/* Exécute une commande interne avec gestion des redirections */
int	execute_builtin_with_redirections(t_cmd *cmd, t_data *data)
{
	int	std_fds[2];

	if (save_std_fds(std_fds) != 0)
		return (1);
	if (setup_redirections(cmd->redirections) != 0)
	{
		restore_std_fds(std_fds);
		return (1);
	}
	int result = execute_builtin(cmd, data);
	restore_std_fds(std_fds); // Restaure les descripteurs après l'exécution
	return (result);
}

/* Exécute une commande unique (interne ou externe) */
int	execute_single_command(t_cmd *cmd, t_data *data)
{
	int	std_fds[2];

	if (!cmd || !cmd->args || !cmd->args[0]) // Vérifie si cmd ou cmd->args est NULL
		return (1);
	if (save_std_fds(std_fds) != 0)
		return (1);
	if (setup_redirections(cmd->redirections) != 0)
	{
		restore_std_fds(std_fds);
		return (1);
	}
	int result;
	if (is_builtin(cmd->args[0]))
		result = execute_builtin(cmd, data);
	else
		result = execute_external(cmd, data);
	restore_std_fds(std_fds); // Restaure les descripteurs après l'exécution
	return (result);
}
