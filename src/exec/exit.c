/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:34:13 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/02 18:34:14 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// void    exit_clean(t_data *data, t_ast *ast)
// {
// 	if (data)
// 	{
// 		if (data->input)
// 			free_tab(data->input);
// 		if (data->env)
// 			free_tab(data->env);
// 		if (data->path)
// 		{
// 			free(data->path);
// 			data->path = NULL;
// 		}
// 	}
// 	if (ast)
// 		free_nodes(ast);
// 	exit(2);
// }