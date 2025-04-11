/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:29:35 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/11 18:10:27 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Récupère la variable d'environnement PATH */
char	*get_path(char **env)
{
	char	*path;
	int		i;

	i = 0;
	path = NULL;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(env[i]);
			if (!path)
				return (NULL);
			return (path);
		}
		i++;
	}
	return (NULL);
}

/* Crée un nœud de la liste chaînée pour une variable d'environnement */
static void	create_env_node(t_env **env_list, char *env_var)
{
	char	*name;
	char	*value;
	char	*equal_sign;

	equal_sign = ft_strchr(env_var, '=');
	if (equal_sign)
	{
		*equal_sign = '\0';
		name = ft_strdup(env_var);
		if (!name)
			return;
		value = ft_strdup(equal_sign + 1);
		if (!value)
		{
			free(name);
			*equal_sign = '=';
			return;
		}
		*equal_sign = '=';
		if (name && value)
			add_value(env_list, name, value);
		else
		{
			if (name)
				free(name);
			if (value)
				free(value);
		}
	}
}

/* Convertit le tableau d'environnement en liste chaînée */
t_env	*env_to_list(char **env)
{
	t_env	*env_list;
	int		i;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		create_env_node(&env_list, env[i]);
		i++;
	}
	return (env_list);
}

static char	*join_path_and_cmd(char *path, char *cmd)
{
	char	*tmp;
	char	*cmd_path;

	tmp = ft_strjoin(path, "/");
	if (!tmp)
		return (NULL);
	cmd_path = ft_strjoin(tmp, cmd);
	free(tmp);
	if (!cmd_path)
		return (NULL);
	return (cmd_path);
}

static char	*search_command_in_paths(char **paths, char *cmd)
{
	char	*cmd_path;
	int		i;

	i = 0;
	while (paths[i])
	{
		cmd_path = join_path_and_cmd(paths[i], cmd);
		if (!cmd_path)
			return (free_tab(paths), NULL);
		if (access(cmd_path, X_OK) == 0)
			return (free_tab(paths), cmd_path);
		free(cmd_path);
		i++;
	}
	return (free_tab(paths), NULL);
}

/* Trouve le chemin complet d'une commande dans le PATH */
char	*find_command_path(char *cmd, t_data *data)
{
	char	**paths;

	if (!cmd || !data || !data->path)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	paths = ft_split(data->path + 5, ':');
	if (!paths)
		return (NULL);
	return (search_command_in_paths(paths, cmd));
}

/* Initialise la structure data avec les paramètres du programme */
t_data	*init_data(int ac, char **av, char **env)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	load_history();
	(void)ac;
	(void)av;
	data->input = NULL;
	data->env = copy_env(env);
	if (!data->env)
		return (free(data), NULL);
	data->env_list = env_to_list(data->env);
	if (!data->env_list)
		return (free_tab(data->env), free(data), NULL);
	data->path = get_path(data->env);
	if (!data->path)
		return (free_tab(data->env),
			free_env_copy(data->env_list), free(data), NULL);
	data->cmd_list = NULL;
	data->exit_status = 0;
	data->pipes = NULL;
	return (data);
}
