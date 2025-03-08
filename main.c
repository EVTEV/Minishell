#include "inc/minishell.h"

int	main(int ac, char **av, char **env)
{
	t_data	*data;

	data = init_data(ac, av, env);
	while (1)
	{
		ft_printf("minishell: ");
		read_input(data);
	}
	return (0);
}
