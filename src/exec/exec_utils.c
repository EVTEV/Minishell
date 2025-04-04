/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:17:23 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/04 10:56:21 by lowatell         ###   ########.fr       */
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
