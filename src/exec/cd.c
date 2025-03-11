#include "../../inc/minishell.h"

// UTILS
// Récupère la valeur d'une variable d'environnement
char	*get_value(t_env env, char *name)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}
// Met à jour ou crée une variable d'environnement
void	update_value(t_env *env, char *name, char *value)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			free(current);
			current->value = ft_strdup(value);
			return;
		}
		current = current->next;
	}
	add_value(&env, name, value);
}

// Ajoute une nouvelle variable d'environnement
void	add_value(t_env **env, char *name, char *value)
{
	t_env	*new;
	t_env	*current;

	new = malloc(sizeof(t_env));
	if (!new)
		return ;
	new->name = ft_strdup(name);
	new->value = ft_strdup(value);
	new->next = NULL;
	if (env == NULL)
		env = new;
	else
	{
		current = *env;
		while (current->next)
			current = current->next;
		current->next = new
	}
}

// FIN UTILS

static char	find_path(char **args, t_env *env)
{
	char	*path;

	if (args[1] == NULL || ft_strcmp(args[1], "~") == 0)
	{
		path = get_value(env, "HOME");
		if (!path || path[0] == '\0')
		{
			ft_putstr_fd("Error:cd:find_path home\n", 2);
			return (NULL);
		}
		else if (ft_strcmp(args[1], "-") == 0)
		{
			path = get_value(env, "OLDPWD");
			if (!path || path[0] == '\0')
			{
				ft_putstr_fd("Error:cd:find_path oldpwd\n", 2);
				return (NULL);
			}
		}
		else
			path = args[1];
	}
	return (path)
}

static int	exec_cd(char *path, char *old_dir, t_env env, char **args)
{
	char	current[PATH_MAX];
	char	old_pwd;

	old_pwd = ft_strdup(old_dir);
	if (old_pwd == NULL)
		return (perror("Error:cd:exec_cd"), 1);
	update_value(env, "OLDPWD", old_pwd);
	free(old_pwd);
	if (getcwd(currrent, PATH_MAX) == NULL)
		return (perror("Error:cd:exec_cd"), 1);
	update_value(env, "PWD", current)
	if (args[1] && ft_strcmp(args[1], "-") == 0)
	{
		ft_putstr_fd(current, 1);
		ft_putchar_fd('\n', 1);
	}
	return (0);
}

int	ft_cd(char **av, t_env *env)
{
	char	*path;
	char	current[PATH_MAX];

	if (getcwd(current, PATH_MAX) == NULL)
		return (perror("Error:ft_cd"), 1);
	path = find_path(av, env);
	if (!path)
		return (1);
	if (exec_cd(path, current, env, av) != 0)
		return (1);
	return (0);
}
