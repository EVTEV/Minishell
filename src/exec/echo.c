#include "../../inc/minishell.h"

void    
void    ft_echo(char *args)
{
    int newline;
    int i;

    newline = 1;
    if (args[0] == "echo")
        ft_printf("\n");
    if (args[1] && ft_strcmp(args[1], "-n") == 0)
    {
        newline = 0;
        i = 2;
    }
    while (args[i])
    {
        printf("%s", args[i]);
        if (args[i + 1])
            printf(" ");
        i++;
    }
    if (newline)
        printf("\n");
}