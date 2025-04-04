/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:50:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 11:22:46 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Gère les erreurs de changement de répertoire */
static int	handle_cd_error(char *path)
{
	ft_putstr_fd("minishell: cd: ", 2);
	if (access(path, F_OK) != 0)
		ft_putstr_fd("No such file or directory\n", 2);
	else if (access(path, X_OK) != 0)
		ft_putstr_fd("Permission denied\n", 2);
	else
		ft_putstr_fd("Not a directory\n", 2);
	return (1);
}

/* Met à jour les variables d'environnement après le changement de répertoire */
static int	update_env_vars(char *old_dir, t_env *env)
{
	char	current[PATH_MAX];
	char	*old_pwd;

	old_pwd = ft_strdup(old_dir);
	if (old_pwd == NULL)
		return (perror("Error:cd:exec_cd"), 1);
	update_value(env, "OLDPWD", old_pwd);
	free(old_pwd);
	if (getcwd(current, PATH_MAX) == NULL)
		return (perror("Error:cd:exec_cd"), 1);
	update_value(env, "PWD", current);
	return (0);
}

/* Exécute le changement de répertoire et met à jour l'environnement */
static int	exec_cd(char *path, char *old_dir, t_env *env, char **args)
{
	if (chdir(path) != 0)
		return (handle_cd_error(path));
	if (update_env_vars(old_dir, env) != 0)
		return (1);
	if (args[1] && ft_strncmp(args[1], "-", 2) == 0)
	{
		ft_putstr_fd(getcwd(NULL, 0), 1);
		ft_putchar_fd('\n', 1);
	}
	return (0);
}

/* Détermine le chemin de destination pour la commande cd */
static char	*find_path(char **args, t_env *env)
{
	char	*path;

	if (args[1] == NULL || ft_strncmp(args[1], "~", 2) == 0)
	{
		path = get_value(*env, "HOME");
		if (!path || path[0] == '\0')
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			return (NULL);
		}
	}
	else if (ft_strncmp(args[1], "-", 2) == 0)
	{
		path = get_value(*env, "OLDPWD");
		if (!path || path[0] == '\0')
		{
			ft_putstr_fd("minishell: cd: OLDPWD not set\n", 2);
			return (NULL);
		}
	}
	else
		path = args[1];
	return (path);
}

/* Fonction principale de la commande cd */
int	ft_cd(char **av, t_env *env)
{
	char	*path;
	char	current[PATH_MAX];

	if (!av[1] || !av[2])
	{
		if (getcwd(current, PATH_MAX) == NULL)
			return (perror("Error:ft_cd"), 1);
		path = find_path(av, env);
		if (!path)
			return (1);
		if (exec_cd(path, current, env, av) != 0)
			return (1);
		return (0);
	}
	ft_putstr_fd("minishell: cd: too many arguments\n", 2);
	return (1);
}
