/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   node_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 16:09:50 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/11 18:31:53 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_nodes(t_ast *root)
{
	if (!root)
		return ;
	ft_printf("type = 1:cmd | 2:pipe | 3:redirect:\n");
	ft_printf("type: %d\n", root->type);
	print_tab(root->args);
	if (root->file)
		ft_printf("file: %s\n", root->file);
	if (root->right)
		print_nodes(root->right);
	if (root->left)
		print_nodes(root->left);
}

char	**realloc_args(char **args, char *new_arg)
{
	char	**new_args;
	int		i;

	if (!new_arg)
		return (args);
	i = 0;
	while (args && args[i])
		i++;
	new_args = (char **)malloc(sizeof(char *) * (i + 2));
	if (!new_args)
		return (free_tab(args), NULL);
	i = 0;
	while (args && args[i])
	{
		new_args[i] = args[i];
		i++;
	}
	new_args[i] = ft_strdup(new_arg);
	if (!new_args[i])
		return (free_tab(new_args), free(args), NULL);
	new_args[i + 1] = NULL;
	return (new_args);
}

t_ast	*create_node(int type, char *file, t_ast *root)
{
	t_ast	*node;

	node = (t_ast *)malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->type = type;
	node->args = NULL;
	if (file)
	{
		node->file = ft_strdup(file);
		if (!node->file)
			return (NULL);
	}
	else
		node->file = NULL;
	node->right = NULL;
	if (!root)
		node->left = NULL;
	else
		node->left = root;
	return (node);
}
