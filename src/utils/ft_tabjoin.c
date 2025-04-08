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
	{
		free_tab(tab);
		free(new_elem);
		return (NULL);
	}
	for (int i = 0; i < len; i++)
	{
		new_tab[i] = ft_strdup(tab[i]);
		if (!new_tab[i]) // Handle allocation failure during duplication
		{
			while (--i >= 0)
				free(new_tab[i]);
			free(new_tab);
			free_tab(tab);
			free(new_elem);
			return (NULL);
		}
	}
	new_tab[len] = ft_strdup(new_elem);
	if (!new_tab[len])
	{
		for (int i = 0; i < len; i++)
			free(new_tab[i]);
		free(new_tab);
		free_tab(tab);
		free(new_elem);
		return (NULL);
	}
	new_tab[len + 1] = NULL;
	free_tab(tab);
	free(new_elem);
	return (new_tab);
}
