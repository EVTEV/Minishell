#ifndef MINISHELL_H
# define MINISHELL_H

# include "../Libft/inc/libft.h"
# include <sys/wait.h>
# include <string.h>
# include <stdio.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/stat.h>
# include <dirent.h>
# include <errno.h>

typedef struct s_env
{
	char			*name;
	char			*value;
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
	t_env	*env_list;
	char	*path;
	t_cmd	*cmd_list;
	int		exit_status;
	int		**pipes;
}	t_data;

// ==================== Pars ==================== //
// --------------- Pars.c --------------- //
t_cmd	*parse_input(char *input, t_data *data);
// --------------- Read.c --------------- //
char	*read_input(t_data *data);

// ==================== Exec ==================== //
// ------------------ builtins --------------- //
// ---------- cd.c ------------ //
int		ft_cd(char **av, t_env *env);
// ---------- echo.c ------------ //
int		ft_echo(int ac, char **av);
// ---------- exit.c ------------ //
void    exit_clean(t_data *data);
// ---------- export.c ------------ //
int		ft_export(char **args, t_env **env);
// ---------- export_utils.c ------------ //
int		is_valid_identifier(char *str);
int		compare_env_vars(t_env *a, t_env *b);
t_env	*copy_env_list(t_env *env);
void	free_env_copy(t_env *env_copy);
// ---------- export_sort.c ------------ //
void	sort_env_list(t_env **head);
void	handle_export_no_value(t_env **env, char *name);
int		print_sorted_env(t_env *env);
// ---------- pwd.c ------------ //
int		ft_pwd(void);
// ---------- unset.c ------------ //
int		ft_unset(char **args, t_env **env);

// ------------------ pipes ------------------ //
// ---------- child_processes.c ------------ //
int		execute_command_in_child(t_cmd *cmd, t_data *data);
int		wait_for_children(pid_t *pids, int cmd_count);
// ---------- heredoc.c ------------ //
int		handle_heredoc(char *delimiter, int *fd_in);
// ---------- pipe_creation.c ------------ //
int		create_pipes(t_data *data, int pipe_count);
pid_t	*allocate_pids(int cmd_count, int pipe_count, t_data *data);
// ---------- pipe_processes.c ------------ //
int		execute_pipe_processes(t_data *data, int cmd_count, int pipe_count);
// ---------- pipes.c ------------ //
int		count_commands(t_cmd *cmd_list);
void	close_all_pipes(t_data *data, int pipe_count);
void	free_pipes(t_data *data, int pipe_count);
int		execute_piped_commands(t_data *data);

// -------------- execution.c ---------------- //
int		is_builtin(char *cmd);
int		execute_builtin(t_cmd *cmd, t_data *data);
int		execute_external(t_cmd *cmd, t_data *data);
int		execute_single_command(t_cmd *cmd, t_data *data);
int		execute_commands(t_data *data);
// -------------- redirections.c -------------- //
int		setup_redirections(t_redir *redirections);

// ==================== Utils ==================== //
// --------------- Utils.c --------------- //
// ---------- free_cmd.c ------------ //
void	free_cmd_list(t_cmd *cmd_list);
// ---------- get_value.c ------------ //
char	*get_value(t_env env, char *name);
void	update_value(t_env *env, char *name, char *value);
void	add_value(t_env **env, char *name, char *value);
// ---------- init_data.c ------------ //
char	**copy_env(char **env);
char	*get_path(char **env);
t_env	*env_to_list(char **env);
char	*find_command_path(char *cmd, t_data *data);
t_data	*init_data(int ac, char **av, char **env);
// ---------- print_tab.c ------------ //
void	print_tab(char **tab);

#endif
