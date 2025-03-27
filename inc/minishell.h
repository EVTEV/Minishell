#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/inc/libft.h"
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>

typedef struct s_env
{
    int				value;
	char			*name;
	struct s_env	*next;
}	t_env;

typedef struct s_redir
{
	int				type;
	char			*file;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirections;
	int				pipe_in;
	int				pipe_out;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_data
{
    char    *input;
	char	**env;
	char	*path;
	t_cmd	*cmd_list;
	int		exit_status;
	int		**pipes;
}	t_data;

// -------------------- Pars --------------------- //
char	*read_input(t_data *data);
t_cmd	*parse_input(char *input, t_data *data);

// -------------------- Exec -------------------- //
void    exit_clean(t_data *data);
int		ft_echo(int ac, char **av);
int		ft_cd(char **av, t_env *env);
int		execute_commands(t_data *data);
int		execute_single_command(t_cmd *cmd, t_data *data);
int		execute_builtin(t_cmd *cmd, t_data *data);
int		execute_external(t_cmd *cmd, t_data *data);
int		execute_piped_commands(t_data *data);
char	*find_command_path(char *cmd, t_data *data);
int		setup_redirections(t_redir *redirections);
int		handle_heredoc(char *delimiter, int *fd_in);
void	close_all_pipes(t_data *data, int pipe_count);
void	free_pipes(t_data *data, int pipe_count);

// -------------------- Utils -------------------- //
t_data	*init_data(int ac, char **av, char **env);
void	print_tab(char **tab);
void	free_cmd_list(t_cmd *cmd_list);
int		is_builtin(char *cmd);

#endif
