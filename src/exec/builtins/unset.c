/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:30:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 14:30:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Supprime une variable de l'environnement */
static void	remove_env_var(t_env **env, char *name)
{
	t_env	*current;
	t_env	*prev;

	if (!env || !*env || !name)
		return ;
	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name) + 1) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->name);
			free(current->value);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/* Supprime des variables de l'environnement */
int	ft_unset(char **args, t_env **env)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!args[i])
		return (0);
	while (args[i])
	{
		if (is_valid_identifier(args[i]))
			remove_env_var(env, args[i]);
		else
		{
			ft_printf("minishell: unset: `%s`: not a valid identifier\n", args[i]);
			status = 1;
		}
		i++;
	}
	return (status);
}