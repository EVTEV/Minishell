#include "../../inc/minishell.h"

/* Vérifie si un chemin est un répertoire */
int	is_directory(const char *path)
{
	struct stat	path_stat;

	if (stat(path, &path_stat) != 0) // Handle inaccessible or non-existent paths
		return (0);
	return (S_ISDIR(path_stat.st_mode));
}
