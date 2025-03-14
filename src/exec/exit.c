#include "../../inc/minishell.h"

void    exit_clean(t_data *data, t_ast **ast)
{
	(void)data;
	free_nodes(*ast);
	exit(2);
}