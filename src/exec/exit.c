#include "../../inc/minishell.h"

// void    exit_clean(t_data *data, t_ast *ast)
// {
// 	if (data)
// 	{
// 		if (data->input)
// 			free_tab(data->input);
// 		if (data->env)
// 			free_tab(data->env);
// 		if (data->path)
// 		{
// 			free(data->path);
// 			data->path = NULL;
// 		}
// 	}
// 	if (ast)
// 		free_nodes(ast);
// 	exit(2);
// }