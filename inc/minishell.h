/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:35:34 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 22:28:17 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/inc/libft.h"
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <sys/types.h>
# include <sys/file.h>
# include <sys/stat.h>
# include <sys/errno.h>

# define QUOTE_ERR "Erreur: quote non fermee\n"

typedef enum	e_token_type
{
	WORD,
	PIPE,
	REDIR_IN,
	REDIR_OUT,
	REDIR_APPEND,
	HEREDOC
}	t_token_type;

typedef struct s_redir {
	char			*file;
	t_token_type	type;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd {
	char			**args;
	t_redir			*redir;
	struct s_cmd	*next;
}	t_cmd;

typedef	struct s_token
{
	char			*value;
	t_token_type	type;
	struct s_token	*next;
}	t_token;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;;


// -------------------- Pars --------------------- //
t_token	*create_token(char *value, t_token_type type);
void	free_tokens(t_token *tokens);
void	add_token(t_token **head, t_token *new_token);
char	*extract_quoted(const char *s, int *i, char quote);
void	add_operator_token(t_token **lst, char *op);
t_token	*tokenize(char *line);
t_env	*init_env(char **envp);
t_env	*add_env_node(t_env *head, t_env *new);
t_env	*create_env_node(char *entry);
char	*append_and_free(char *s1, char *s2);
char	*get_var_name(const char *s, int *i);
char	*get_var_value(char *name, t_env *env, int status);
char 	*expand_string(const char *input, t_env *env, int last_status);
int 	expand_tokens(t_token *tokens, t_env *env, int last_status);
t_redir	*create_redir(char *file, t_token_type type);
void	add_redir(t_redir **list, t_redir *new);
char	**fill_args(t_token **tok, t_env *env, int status, int count);
char	**extract_args(t_token **tok, t_env *env, int status);
t_redir	*extract_redirs(t_token **tok);

// -------------------- Exec -------------------- //
// void    exit_clean(t_data *data, t_ast **ast);
// void	ft_echo(char **av);
// int		ft_cd(char **av, t_env *env);

// -------------------- Utils -------------------- //
void	free_args(char **arr);
int		is_var_start(char c);
t_redir	*create_redir(char *file, t_token_type type);
t_cmd	*create_cmd(t_token **tok, t_env *env, int status);
t_cmd	*parse_tokens(t_token *tokens, t_env *env, int status);
void	free_cmds(t_cmd *cmd);
int		count_args(t_token *token);
void	print_tab(char **tab);
void	free_env(t_env *node);
char	*ft_strndup(const char *s, size_t n);
void	print_cmds(t_cmd *cmds);

#endif
