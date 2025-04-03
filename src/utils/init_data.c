/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 16:29:35 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/08 16:53:15 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Copie le tableau d'environnement dans une nouvelle allocation */
char	**copy_env(char **env)
{
	int		i;
	char	**cpy_env;

	i = 0;
	while (env[i])
		i++;
	cpy_env = (char **)malloc(sizeof(char *) * (i + 1));
	if (!cpy_env)
		return (NULL);
	i = 0;
	while (env[i])
	{
		cpy_env[i] = ft_strdup(env[i]);
		if (!cpy_env[i])
			return (free_tab(cpy_env), NULL);
		i++;
	}
	return (cpy_env);
}

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

/* Convertit le tableau d'environnement en liste chaînée */
t_env	*env_to_list(char **env)
{
	t_env	*env_list;
	int		i;
	char	*name;
	char	*value;
	char	*equal_sign;

	env_list = NULL;
	i = 0;
	while (env[i])
	{
		equal_sign = ft_strchr(env[i], '=');
		if (equal_sign)
		{
			*equal_sign = '\0';
			name = ft_strdup(env[i]);
			value = ft_strdup(equal_sign + 1);
			*equal_sign = '=';
			if (name && value)
				add_value(&env_list, name, value);
			else
			{
				free(name);
				free(value);
			}
		}
		i++;
	}
	return (env_list);
}

/* Trouve le chemin complet d'une commande dans le PATH */
char	*find_command_path(char *cmd, t_data *data)
{
	char	**paths;
	char	*tmp;
	char	*cmd_path;
	int		i;

	if (!cmd || !data || !data->path)
		return (NULL);
	if (cmd[0] == '/' || cmd[0] == '.')
		return (ft_strdup(cmd));
	paths = ft_split(data->path + 5, ':');
	if (!paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(cmd_path, X_OK) == 0)
			return (free_tab(paths), cmd_path);
		free(cmd_path);
		i++;
	}
	return (free_tab(paths), NULL);
}

/* Initialise la structure data avec les paramètres du programme */
t_data	*init_data(int ac, char **av, char **env)
{
	t_data	*data;

	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	(void)ac;
	(void)av;
	data->input = NULL;
	data->env = copy_env(env);
	if (!data->env)
		return (free(data), NULL);
	data->env_list = env_to_list(data->env);
	data->path = get_path(data->env);
	if (!data->path)
		return (free(data->env), free(data), NULL);
	data->cmd_list = NULL;
	data->exit_status = 0;
	data->pipes = NULL;
	return (data);
}