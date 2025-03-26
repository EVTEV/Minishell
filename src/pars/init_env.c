/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:19:12 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 22:16:29 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_env	*create_env_node(char *entry)
{
	t_env	*node;
	char	*key;
	char	*value;
	int		i;

	i = 0;
	while (entry[i] && entry[i] != '=')
		i++;
	if (!entry[i])
		return (NULL);
	key = ft_substr(entry, 0, i);
	if (!key)
		return (NULL);
	value = ft_strdup(entry + i + 1);
	if (!value)
		return (free(key), NULL);
	node = (t_env *)malloc(sizeof(t_env));
	if (!node)
		return (free(key), free(value), NULL);
	node->key = key;
	node->value = value;
	node->next = NULL;
	return (node);
}

t_env	*add_env_node(t_env *head, t_env *new)
{
	t_env	*tmp;

	if (!head)
		return (new);
	tmp = head;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
	return (head);
}

t_env	*init_env(char **envp)
{
	t_env	*head;
	t_env	*new;
	int		i;

	i = 0;
	head = NULL;
	while (envp[i])
	{
		new = create_env_node(envp[i]);
		if (new)
			head = add_env_node(head, new);
		else
			return (free_env(head), NULL);
		i++;
	}
	return (head);
}

void	free_env(t_env *node)
{
	t_env	*tmp;

	while (node)
	{
		tmp = node->next;
		free(node->value);
		free(node->key);
		free(node);
		node = tmp;
	}
}
