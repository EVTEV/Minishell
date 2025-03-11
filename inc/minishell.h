/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 18:35:34 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/11 18:37:07 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/inc/libft.h"

# define CMD 1
# define PIPE 2
# define REDIR 3

typedef	struct s_env
{
    int				value;
	char			*name;
	struct s_env	*next;
}	t_env;

typedef	struct s_data
{
    char    **input;
	char	**env;
	char	*path;
	
}	t_data;

typedef	struct s_ast
{
	int				type;
	char			**args;
	char			*file;
	struct s_ast	*right;
	struct s_ast	*left;
}	t_ast;

// -------------------- Pars --------------------- //
char	**read_input(t_data *data);
t_ast	*pars_command(char **token);
int		assign_redirect_type(char *redirect);

// -------------------- Exec -------------------- //
void    exit_clean(t_data *data);
int		ft_echo(int ac, char **av);
int		ft_cd(char **av, t_env *env);

// -------------------- Utils -------------------- //
t_data	*init_data(int ac, char **av, char **env);
t_ast	*create_node(int type, char *file, t_ast *root);
char	**realloc_args(char **args, char *new_arg);
void	print_tab(char **tab);
void	print_nodes(t_ast *root);

#endif
