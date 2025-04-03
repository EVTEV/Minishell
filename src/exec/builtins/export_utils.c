/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:30:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 14:30:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Vérifie si un nom de variable est valide */
int	is_valid_identifier(char *str)
{
	int	i;

	if (!str || !str[0])
		return (0);
	if (!ft_isalpha(str[0]) && str[0] != '_')
		return (0);
	i = 1;
	while (str[i])
	{
		if (!ft_isalnum(str[i]) && str[i] != '_')
			return (0);
		i++;
	}
	return (1);
}

/* Compare deux noms de variables pour le tri */
int	compare_env_vars(t_env *a, t_env *b)
{
	return (ft_strncmp(a->name, b->name, ft_strlen(a->name) + 1));
}

/* Copie la liste d'environnement pour le tri */
t_env	*copy_env_list(t_env *env)
{
	t_env	*env_copy;
	t_env	*current;

	env_copy = NULL;
	current = env;
	while (current)
	{
		add_value(&env_copy, current->name, current->value);
		current = current->next;
	}
	return (env_copy);
}

/* Libère la liste d'environnement copiée */
void	free_env_copy(t_env *env_copy)
{
	t_env	*current;

	while (env_copy)
	{
		current = env_copy;
		env_copy = env_copy->next;
		free(current->name);
		free(current->value);
		free(current);
	}
}
