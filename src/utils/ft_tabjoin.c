#include "../../inc/minishell.h"

char	**ft_tabjoin(char **tab, char *new_elem)
{
	int		len = 0;
	char	**new_tab;

	if (tab)
		while (tab[len])
			len++;
	new_tab = malloc(sizeof(char *) * (len + 2));
	if (!new_tab)
		return (NULL);
	for (int i = 0; i < len; i++)
		new_tab[i] = tab[i];
	new_tab[len] = ft_strdup(new_elem);
	new_tab[len + 1] = NULL;
	free(tab);
	return (new_tab);
}
