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
		if (input[i] == '"') // Handle double quotes
		{
			tmp = ft_substr(input, i++, 1);
			result = ft_strjoin_free(result, tmp);
			free(tmp);
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
				{
					char *char_to_add = ft_substr(input, i++, 1); // Extract single character
					result = ft_strjoin_free(result, char_to_add); // Append to result
					free(char_to_add); // Free the temporary string
				}
			}
			if (!input[i]) // If closing quote is missing
			{
				ft_putstr_fd("minishell: syntax error: unclosed double quote\n", STDERR_FILENO);
				free(result);
				return (NULL);
			}
			if (input[i] == '"')
			{
				char *closing_quote = ft_substr(input, i++, 1); // Extract closing quote
				result = ft_strjoin_free(result, closing_quote); // Append to result
				free(closing_quote); // Free the temporary string
			}
		}
		else if (input[i] == '\'') // Handle single quotes
		{
			tmp = ft_substr(input, i++, 1); // Extract opening single quote
			result = ft_strjoin_free(result, tmp); // Append to result
			free(tmp);
			start = i;
			while (input[i] && input[i] != '\'')
				i++;
			if (!input[i]) // If closing quote is missing
			{
				ft_putstr_fd("minishell: syntax error: unclosed single quote\n", STDERR_FILENO);
				free(result);
				return (NULL);
			}
			tmp = ft_substr(input, start, i - start); // Extract content inside quotes
			result = ft_strjoin_free(result, tmp);
			free(tmp);
			if (input[i] == '\'')
			{
				tmp = ft_substr(input, i++, 1); // Extract closing single quote
				result = ft_strjoin_free(result, tmp); // Append to result
				free(tmp);
			}
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
			char *single_dollar = ft_strdup("$"); // Create a temporary string for "$"
			result = ft_strjoin_free(result, single_dollar); // Append to result
			free(single_dollar); // Free the temporary string
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
