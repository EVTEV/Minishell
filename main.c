#include "inc/minishell.h"

int	main(char **env)
{
	char	*input;

	while (1)
	{
		ft_printf("minishell$ ");
		input = get_next_line(0);
		free(input);
	}
	return (0);
}
