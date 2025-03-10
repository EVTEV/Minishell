#include "../../inc/minishell.h"

static int	check_args(char	**args, int	*start)
{
	int	i;

	i = 1;
	while (args[i] && ft_strcmp(str, "-n") == 0)
		i++;
	*start = i;
	if (i > 1)
		return (1);
	return (0);
}

static void	write_args(char **args, int start, int newline)
{
	int	i;

	i = start;
	while (args[i])
	{
		ft_printf("%s", args[i]);
		if (args[i + 1])
			ft_printf(" ");
		i++;
	}
	if (newline)
		ft_printf("\n");
}

int	ft_echo(int ac, char **av)
{
	int	newline;
	int	start;

	if (ac < 2)
	{
		ft_printf("\n");
		return (0);
	}
	newline = !check_args(av, &start);
	write_args(av, start, newline);
	return (0);
}
