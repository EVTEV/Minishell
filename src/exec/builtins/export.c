/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:30:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/11 19:04:57 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Gère l'ajout ou mise à jour d'une variable */
static int	handle_env_var(t_env **env, char *name, char *value)
{
	t_env	*existing;

	existing = *env;
	while (existing
		&& ft_strncmp(existing->name, name, ft_strlen(name) + 1) != 0)
		existing = existing->next;
	if (existing)
		update_value(*env, name, value);
	else if (!add_value(env, name, value))
		return (1);
	return (0);
}

/* Affiche un message d'erreur pour un identifiant invalide */
static void	print_invalid_identifier_error(char *name)
{
	ft_putstr_fd("minishell: export: `", STDERR_FILENO);
	ft_putstr_fd(name, STDERR_FILENO);
	ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
}

/* Gère l'ajout ou mise à jour d'une variable avec une valeur */
static int	handle_env_var_with_value(char *name, char *value, t_env **env)
{
	if (*value)
	{
		if (handle_env_var(env, name, value))
			return (1);
	}
	else
	{
		if (handle_env_var(env, name, ""))
			return (1);
	}
	return (0);
}

/* Traite un argument contenant un signe égal */
static int	process_export_with_equal(char *arg, t_env **env)
{
	char	*equal_sign;
	char	*name;
	char	*value;
	int		status;

	equal_sign = ft_strchr(arg, '=');
	if (!equal_sign)
		return (0);
	*equal_sign = '\0';
	name = arg;
	value = equal_sign + 1;
	if (is_valid_identifier(name))
		status = handle_env_var_with_value(name, value, env);
	else
	{
		print_invalid_identifier_error(name);
		status = 1;
	}
	*equal_sign = '=';
	return (status);
}

/* Traite un argument de la commande export */
static int	process_export_arg(char **args, int i, t_env **env)
{
	if (!args[i])
		return (0);
	if (ft_strchr(args[i], '='))
		return (process_export_with_equal(args[i], env));
	else if (!is_valid_identifier(args[i]))
	{
		ft_putstr_fd("minishell: export: `", STDERR_FILENO);
		ft_putstr_fd(args[i], STDERR_FILENO);
		ft_putstr_fd("': not a valid identifier\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}

/* Gère la commande export (avec ou sans arguments) */
int	ft_export(char **args, t_env **env)
{
	int	i;
	int	status;
	int	ret_code;

	ret_code = 0;
	if (!args[1])
		return (print_sorted_env(*env));
	i = 1;
	status = 0;
	while (args[i])
	{
		status = process_export_arg(args, i, env);
		if (status != 0)
			ret_code = 1;
		i++;
	}
	return (ret_code);
}
