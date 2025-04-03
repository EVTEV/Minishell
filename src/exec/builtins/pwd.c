/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:30:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 14:30:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Affiche le répertoire de travail actuel */
int	ft_pwd(void)
{
	char	path[4096];

	if (getcwd(path, sizeof(path)) != NULL)
	{
		ft_printf("%s\n", path);
		return (0);
	}
	else
	{
		ft_printf("minishell: pwd: %s\n", strerror(errno));
		return (1);
	}
}