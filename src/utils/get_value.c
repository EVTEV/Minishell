/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_value.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 10:03:19 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Récupère la valeur d'une variable d'environnement par son nom */
char	*get_value(t_env env, char *name)
{
	t_env	*current;

	current = &env;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name) + 1) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}

/* Met à jour la valeur d'une variable d'environnement existante */
void	update_value(t_env *env, char *name, char *value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strncmp(current->name, name, ft_strlen(name) + 1) == 0)
		{
			free(current->value);
			current->value = ft_strdup(value);
			return ;
		}
		current = current->next;
	}
	add_value(&env, name, value);
}

/* Ajoute une nouvelle variable d'environnement à la liste */
void	add_value(t_env **env, char *name, char *value)
{
	t_env	*new;
	t_env	*current;

	new = (t_env *)malloc(sizeof(t_env));
	if (!new)
		return ;
	new->name = name;
	new->value = value;
	new->next = NULL;
	if (*env == NULL)
		*env = new;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new;
	}
}
