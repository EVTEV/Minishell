#include "inc/minishell.h"

int	main(int ac, char **av)
{
	if (av[0] && ac > 0)
		return (0);
	return (2);
}
