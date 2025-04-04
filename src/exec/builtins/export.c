/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:30:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 11:24:03 by lowatell         ###   ########.fr       */
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
	else
		add_value(env, name, value);
	return (0);
}

/* Traite un argument contenant un signe égal */
static int	process_export_with_equal(char *arg, t_env **env)
{
	char	*equal_sign;
	char	*name;
	char	*value;

	equal_sign = ft_strchr(arg, '=');
	*equal_sign = '\0';
	name = arg;
	value = equal_sign + 1;
	if (is_valid_identifier(name))
	{
		handle_env_var(env, name, value);
		*equal_sign = '=';
		return (0);
	}
	else
	{
		ft_printf("minishell: export: `%s': not a valid identifier\n", name);
		*equal_sign = '=';
		return (1);
	}
}

/* Traite un argument de la commande export */
static int	process_export_arg(char **args, int i, t_env **env)
{
	if (ft_strchr(args[i], '='))
		return (process_export_with_equal(args[i], env));
	else if (is_valid_identifier(args[i]))
		handle_export_no_value(env, args[i]);
	return (0);
}

/* Gère la commande export (avec ou sans arguments) */
int	ft_export(char **args, t_env **env)
{
	int	i;
	int	status;

	if (!args[1])
		return (print_sorted_env(*env));
	i = 1;
	status = 0;
	while (args[i])
	{
		status = process_export_arg(args, i, env);
		i++;
	}
	return (status);
}
