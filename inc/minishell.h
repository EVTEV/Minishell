/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:35:34 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/02 16:38:57 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/inc/libft.h"
# include "token.h"
# include "env.h"
# include "ast.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/file.h>
# include <sys/stat.h>
# include <sys/errno.h>
# include <signal.h>


// -------------------- Pars --------------------- //
t_token		*lexer(char *input);
t_ast_node	*parser(t_token *tokens);
void		free_ast(t_ast_node *root);
void		free_tokens(t_token *head);
void		expand_tokens(t_token *tokens, t_env *env);

// -------------------- Exec -------------------- //

// -------------------- Signals -------------------- //

// -------------------- Utils -------------------- //

#endif
