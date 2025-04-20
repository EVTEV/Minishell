#include "../../inc/minishell.h"

void    ft_clean_all(int sig)
{
    t_data *data;

    data = (t_data *)(long)g_exit_status;
    g_exit_status = 130;
    if (sig == SIGINT)
    {
        free_cmd_list((t_cmd *)(long)data->tmp);
        exit_clean(data, data->tokens, g_exit_status);
    }
}

void signnn(t_data *data, int i)
{
    
    data->exit_status = g_exit_status;
    data->tmp = i;
    g_exit_status = (int)(long)data;
    signal(SIGINT, ft_clean_all);
}
