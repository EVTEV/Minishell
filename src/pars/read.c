/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:46:42 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/14 18:15:45 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**read_input()
{
	char	*input;
	char	**tab;

	input = readline("minishell$ ");
	if (!input)
	{
		input = ft_strdup("");
		if (!input)
			return (NULL);
	}
	if (ft_strlen(input))
		add_history(input);
	tab = ft_split(input, ' ');
	if (!tab)
		return (free(input), input = NULL, NULL);
	free(input);
	return (tab);
}
