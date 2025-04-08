/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/04 10:08:16 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/04 10:08:21 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**copy_env(char **env)
{
	int		i;
	char	**cpy_env;

	i = 0;
	while (env[i])
		i++;
	cpy_env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!cpy_env)
		return (NULL);
	cpy_env[i] = NULL; // Ensure the last element is NULL
	i = 0;
	while (env[i])
	{
		cpy_env[i] = ft_strdup(env[i]);
		if (!cpy_env[i])
			return (free_tab(cpy_env), NULL);
		i++;
	}
	return (cpy_env);
}
