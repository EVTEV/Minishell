/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:31:05 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/02 20:13:20 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_env_value(t_env *env, char *key)
{
	while (env)
	{
		if (ft_strncmp(env->key, key, ft_strlen(key)) == 0
			&& ft_strlen(env->key) == ft_strlen(key))
			return (env->value);
		env = env->next;
	}
	return (NULL);
}

void	free_env(t_env *env)
{
	t_env	*tmp;

	while (env)
	{
		tmp = env;
		env = env->next;
		free(tmp->key);
		free(tmp->value);
		free(tmp);
	}
}

t_env	*create_env_node(char *env_entry)
{
	t_env	*new_node;
	char	*equal_sign;
	char	*key;
	char	*value;

	equal_sign = ft_strchr(env_entry, '=');
	if (!equal_sign)
		return (NULL);
	key = ft_substr(env_entry, 0, equal_sign - env_entry);
	value = ft_strdup(equal_sign + 1);
	new_node = malloc(sizeof(t_env));
	if (!new_node || !key || !value)
	{
		free(key);
		free(value);
		free(new_node);
		return (NULL);
	}
	new_node->key = key;
	new_node->value = value;
	new_node->next = NULL;
	return (new_node);
}

t_env	*init_env(char **envp)
{
	t_env	*env;
	t_env	*new_node;
	int		i;

	if (!envp)
		return (NULL);
	env = NULL;
	i = 0;
	while (envp[i])
	{
		new_node = create_env_node(envp[i]);
		if (!new_node)
		{
			free_env(env);
			return (NULL);
		}
		new_node->next = env;
		env = new_node;
		i++;
	}
	return (env);
}
