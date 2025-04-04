#include "../../inc/minishell.h"

static char	*expand_variable(char *input, t_env *env_list, int *len)
{
	char	*name;
	char	*value;
	int		i = 1;

	// Identifier le nom de la variable
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	*len = i; // Longueur de la variable
	name = ft_substr(input, 1, i - 1);
	if (!name || !*name) // Si le nom est vide
	{
		free(name);
		return (ft_strdup("$")); // Retourne simplement "$"
	}
	value = get_value(*env_list, name); // Récupérer la valeur de la variable
	free(name);
	return (value ? ft_strdup(value) : ft_strdup("")); // Retourne une chaîne vide si la variable n'existe pas
}

char	*expander(char *input, t_data *data)
{
	char	*result = NULL;
	char	*tmp;
	int		i = 0, start, len;

	while (input[i])
	{
		if (input[i] == '\'') // Skip expansion inside single quotes
		{
			start = ++i;
			while (input[i] && input[i] != '\'')
				i++;
			tmp = ft_substr(input, start, i - start);
			result = ft_strjoin_free(result, tmp);
			if (input[i] == '\'')
				i++;
		}
		else if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
		{
			tmp = expand_variable(&input[i], data->env_list, &len);
			result = ft_strjoin_free(result, tmp);
			i += len; // Avancer de la longueur de la variable
		}
		else if (input[i] == '$' && input[i + 1] == '?') // Gestion de $? (statut de sortie)
		{
			tmp = ft_itoa(data->exit_status); // Convertir le statut en chaîne
			result = ft_strjoin_free(result, tmp);
			i += 2; // Avancer de 2 pour "$?"
		}
		else if (input[i] == '$' && (!input[i + 1] || input[i + 1] == ' ')) // Cas de "$" seul
		{
			result = ft_strjoin_free(result, ft_strdup("$"));
			i++; // Avancer de 1 pour "$"
		}
		else
		{
			start = i;
			while (input[i] && input[i] != '$' && input[i] != '\'')
				i++;
			tmp = ft_substr(input, start, i - start);
			result = ft_strjoin_free(result, tmp);
		}
	}
	return (result);
}
