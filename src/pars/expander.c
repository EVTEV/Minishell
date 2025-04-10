#include "../../inc/minishell.h"

static char	*expand_variable(char *input, t_env *env_list, int *len)
{
	char	*name;
	char	*value;
	int		i;

	i = 1;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	*len = i;
	name = ft_substr(input, 1, i - 1);
	if (!name || !*name)
	{
		free(name);
		return (ft_strdup("$"));
	}
	value = get_value(*env_list, name);
	free(name);
	return (value ? ft_strdup(value) : ft_strdup(""));
}

char	*expander(char *input, t_data *data)
{
	char	*result = NULL;
	char	*tmp;
	int		i = 0, start, len;

	while (input[i])
	{
		if (input[i] == '"') // Conserve les guillemets doubles
		{
			result = ft_strjoin_free(result, ft_substr(input, i++, 1)); // Ajoute le guillemet ouvrant
			start = i;
			while (input[i] && input[i] != '"')
			{
				if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
				{
					tmp = expand_variable(&input[i], data->env_list, &len);
					result = ft_strjoin_free(result, tmp);
					free(tmp);
					i += len;
				}
				else if (input[i] == '$' && input[i + 1] == '?') // Gère "$?" (exit status)
				{
					tmp = ft_itoa(data->exit_status);
					result = ft_strjoin_free(result, tmp);
					free(tmp);
					i += 2;
				}
				else
					result = ft_strjoin_free(result, ft_substr(input, i++, 1)); // Ajoute les autres caractères
			}
			if (input[i] == '"')
				result = ft_strjoin_free(result, ft_substr(input, i++, 1)); // Ajoute le guillemet fermant
		}
		else if (input[i] == '\'') // Conserve les guillemets simples
		{
			result = ft_strjoin_free(result, ft_substr(input, i++, 1)); // Ajoute le guillemet ouvrant
			start = i;
			while (input[i] && input[i] != '\'')
				i++;
			tmp = ft_substr(input, start, i - start); // Extrait le contenu entre les guillemets
			result = ft_strjoin_free(result, tmp);
			free(tmp);
			if (input[i] == '\'')
				result = ft_strjoin_free(result, ft_substr(input, i++, 1)); // Ajoute le guillemet fermant
		}
		else if (input[i] == '$' && (ft_isalnum(input[i + 1]) || input[i + 1] == '_'))
		{
			tmp = expand_variable(&input[i], data->env_list, &len);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
			i += len;
		}
		else if (input[i] == '$' && input[i + 1] == '?') // Gère "$?" (exit status)
		{
			tmp = ft_itoa(data->exit_status);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
			i += 2;
		}
		else if (input[i] == '$' && (!input[i + 1] || input[i + 1] == ' ')) // Gère "$" seul
		{
			result = ft_strjoin_free(result, ft_strdup("$"));
			i++;
		}
		else
		{
			start = i;
			while (input[i] && input[i] != '$' && input[i] != '"' && input[i] != '\'')
				i++;
			tmp = ft_substr(input, start, i - start);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
		}
	}
	return (result);
}
