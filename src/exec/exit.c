#include "../../inc/minishell.h"

void    exit_clean(t_data *data)
{
	int	i;
	int	pipe_count;

	if (!data)
		exit(2);
	if (data->input)
		free(data->input);
	if (data->env)
		free_tab(data->env);
	if (data->path)
		free(data->path);
	if (data->cmd_list)
		free_cmd_list(data->cmd_list);
	
	// Nettoyage des pipes
	if (data->pipes)
	{
		pipe_count = 0;
		if (data->cmd_list)
		{
			i = 0;
			while (data->cmd_list->next)
			{
				data->cmd_list = data->cmd_list->next;
				i++;
			}
			pipe_count = i;
		}
		free_pipes(data, pipe_count);
	}
	
	free(data);
	exit(2);
}