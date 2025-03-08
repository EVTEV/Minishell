/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:46:42 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/08 17:27:30 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*read_input(t_data *data)
{
	char	*input;
	char	**tab;

	tab = NULL;
	input = get_next_line(0);
	if (ft_strncmp(input, "\n", ft_strlen(input) - 1) == 0)
		return (free(input), input = NULL, NULL);
	else if (ft_strncmp(input, "exit", ft_strlen(input) - 1) == 0)
		return (free(input), input = NULL, exit_clean(data), NULL);
	else if (ft_strncmp(input, "env", ft_strlen(input) - 1) == 0)
		print_tab(data->env);
	else if (ft_strncmp(input, "$PATH", ft_strlen(input) - 1) == 0)
		ft_printf("%s\n", data->path);
	// else if (ft_strncmp(input, "echo", 4) == 0)
	// {
	// 	tab = ft_split(input, ' ');
	// 	if (!tab)
	// 		return (free(input), input = NULL, NULL);
	// 	free(input);
	// 	ft_echo()
	// }
	return (input);
}