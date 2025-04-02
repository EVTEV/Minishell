#include "minishell.h"

int	is_valid_env_char(char c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_'); // First character must be a letter or '_'
	return (ft_isalnum(c) || c == '_'); // Subsequent characters can be alphanumeric or '_'
}

char	*expand_variable(char *input, t_env *env)
{
	char	*expanded;
	char	*value;

	if (ft_isdigit(input[1])) // If the first character after '$' is a digit
		return (ft_strdup(input + 2)); // Skip both '$' and the digit
	if (!is_valid_env_char(input[1], 1)) // Check if the first character after '$' is valid
		return (ft_strdup(input + 1)); // Skip the '$' and treat the rest as normal text
	value = get_env_value(env, input + 1);
	if (!value)
		return (ft_strdup(""));
	expanded = ft_strdup(value);
	return (expanded);
}

char	*expand_token(char *input, t_env *env)
{
	char	*expanded;

	if (input[0] == '\'' && input[ft_strlen(input) - 1] == '\'') // Check for single quotes
		return (ft_substr(input, 1, ft_strlen(input) - 2)); // Remove the quotes without expanding
	if (input[0] == '$')
		expanded = expand_variable(input, env);
	else
		expanded = ft_strdup(input);
	return (expanded);
}

void	expand_tokens(t_token *tokens, t_env *env)
{
	char	*expanded;

	while (tokens)
	{
		expanded = expand_token(tokens->value, env);
		free(tokens->value);
		tokens->value = expanded;
		tokens = tokens->next;
	}
}
