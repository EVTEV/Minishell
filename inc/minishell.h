/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:35:34 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/02 20:19:37 by lowatell         ###   ########.fr       */
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
t_ast_node	*create_ast_node(t_token *token);
t_token		*duplicate_token(t_token *token);
char		*remove_quotes(char *value);
t_env		*init_env(char **envp);
char		*expand_var_in_quo(char *result, char *input, int *i, t_env *env);

// -------------------- Exec -------------------- //
// -------------------- Signals -------------------- //
// -------------------- Utils -------------------- //
void		free_ast(t_ast_node *root);
char		*get_env_value(t_env *env, char *key);
char		*expand_variable(char *input, t_env *env);
int			is_valid_env_char(char c, int is_first);
void		free_env(t_env *env);
void		remove_right_node(t_ast_node *current);
char		*ft_strjoin_free(char *s1, const char *s2);

#endif
