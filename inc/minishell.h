#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/inc/libft.h"

typedef struct s_env
{
    int				value;
	char			*name;
	struct s_env	*next;
}	t_env;

typedef struct s_data
{
    char    *input;
	char	**env;
	char	*path;
	
}	t_data;

// -------------------- Pars --------------------- //
char	*read_input(t_data *data);

// -------------------- Exec -------------------- //
void    exit_clean(t_data *data);
int		ft_echo(int ac, char **av);
int		ft_cd(char **av, t_env *env);

// -------------------- Utils -------------------- //
t_data	*init_data(int ac, char **av, char **env);
void	print_tab(char **tab);

#endif
