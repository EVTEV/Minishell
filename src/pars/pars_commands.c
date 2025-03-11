/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars_commands.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 15:33:47 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/11 18:28:07 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	assign_redirect_type(char *redirect)
{
	if (ft_strncmp(redirect, ">", 1) == 0)
		return (1);
	else if (ft_strncmp(redirect, ">>", 2) == 0)
		return (2);
	else if (ft_strncmp(redirect, "<", 1) == 0)
		return (3);
	return (-1);
}

t_ast	*handle_pipe(char **token, int i, t_ast *root)
{
	t_ast	*node;

	node = create_node(PIPE, NULL, root);
	if (!node)
		return (NULL);
	node->right = pars_command(&token[i + 1]);
	return (node);
}

t_ast	*handle_redirect(char **token, int *i, t_ast *root)
{
	if (!token[*i + 1])
		return (NULL);
	return (create_node(REDIR, token[++(*i)], root));
}

void	add_arg_to_cmd(t_ast **root, char *arg)
{
	if (!*root)
		*root = create_node(CMD, NULL, NULL);
	(*root)->args = realloc_args((*root)->args, arg);
}

t_ast	*pars_command(char **token)
{
	t_ast	*root;
	int		i;

	root = NULL;
	i = 0;
	while (token[i])
	{
		if (ft_strncmp(token[i], "|", 1) == 0)
			return (handle_pipe(token, i, root));
		else if (assign_redirect_type(token[i]) != -1)
			root = handle_redirect(token, &i, root);
		else
			add_arg_to_cmd(&root, token[i]);
		i++;
	}
	return (root);
}
