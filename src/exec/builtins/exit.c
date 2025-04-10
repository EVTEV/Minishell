/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 10:56:28 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Libère tous les membres de la structure data */
static void	free_data_members(t_data *data)
{
	if (data->input)
		free(data->input);
	if (data->env)
		free_tab(data->env);
	if (data->env_list)
		free_env_copy(data->env_list);
	if (data->path)
		free(data->path);
	if (data->cmd_list)
	{
		free_cmd_list(data->cmd_list);
		data->cmd_list = NULL; // Prevent double free by nullifying the pointer
	}
}

/* Nettoie et libère toutes les ressources avant de quitter le programme */
void	exit_clean(t_data *data, t_token *tokens, int i)
{
	if (!data && !tokens)
		exit(i);
	if (tokens)
	{
		free_token(tokens);
		tokens = NULL;
	}
	if (data)
		free_data_members(data);
	if (data->pipes)
	{
		if (data->cmd_list)
			free_pipes(data, count_commands(data->cmd_list) - 1);
		else
			free_pipes(data, 0);
	}
	if (data)
	{
		free(data);
		data = NULL; // Prevent double free by nullifying the pointer
	}
	exit(i);
}

/* Vérifie si une chaîne est un argument numérique valide */
static int	is_numeric_argument(char *arg)
{
	int	i = 0;

	if (!arg || !*arg)
		return (0);
	if (arg[i] == '+' || arg[i] == '-')
		i++;
	while (arg[i])
	{
		if (!ft_isdigit(arg[i]))
			return (0);
		i++;
	}
	return (1);
}

/* Gère la commande exit */
void	ft_exit(char **args, t_data *data)
{
	int	exit_code;

	ft_putstr_fd("exit\n", 1);
	if (!args[1]) // Pas d'argument
		exit_clean(data, NULL, data->exit_status);
	if (!is_numeric_argument(args[1])) // Argument non numérique
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(args[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit_clean(data, NULL, 2);
	}
	if (args[2]) // Trop d'arguments
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		data->exit_status = 1; // Set exit status to 1
		return ;
	}
	exit_code = ft_atoi(args[1]) % 256; // Calculer le code de sortie
	if (exit_code < 0)
		exit_code += 256; // Ajuster pour les valeurs négatives
	exit_clean(data, NULL, exit_code);
}
