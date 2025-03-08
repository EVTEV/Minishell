#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/inc/libft.h"

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

// -------------------- Utils -------------------- //
t_data	*init_data(int ac, char **av, char **env);
void	print_tab(char **tab);

#endif
