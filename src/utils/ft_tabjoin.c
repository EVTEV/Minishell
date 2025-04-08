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
		free_tab(tab); // Free the old tab if allocation fails
		free(new_elem);
		return (NULL);
	}
	for (int i = 0; i < len; i++)
		new_tab[i] = tab[i]; // Transfer ownership of pointers from old tab
	new_tab[len] = ft_strdup(new_elem); // Duplicate new_elem
	if (!new_tab[len])
	{
		free(new_tab);
		free_tab(tab); // Free the old tab if duplication fails
		return (NULL);
	}
	free(new_elem); // Free new_elem after duplicating it
	new_tab[len + 1] = NULL;
	free(tab); // Free the old tab pointer after successful transfer
	return (new_tab);
}
