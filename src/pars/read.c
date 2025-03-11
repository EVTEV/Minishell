/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:46:42 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/11 17:34:15 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**read_input(t_data *data)
{
	char	*input;
	char	**tab;

	input = get_next_line(0);
	if (!input)
		return (NULL);
	if (ft_strncmp(input, "\n", 2) == 0)
		return (free(input), NULL);
	tab = ft_split(input, ' ');
	if (!tab)
		return (free(input), free(data), NULL);
	free(input);
	return (tab);
}
