/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 16:30:16 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/03 17:57:06 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef AST_H
# define AST_H

# include "minishell.h"

typedef struct s_ast_node
{
	t_token				*token;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}	t_ast_node;

#endif
