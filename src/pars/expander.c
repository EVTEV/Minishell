/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:31:03 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/02 20:16:24 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_variable(char *input, t_env *env)
{
	char	*value;

	if (!input || !*input || input[0] != '$')
		return (ft_strdup(input));
	value = get_env_value(env, input + 1);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
}

static char	*append_character(char *result, char c)
{
	char	*tmp;
	char	*char_str;

	char_str = ft_substr(&c, 0, 1);
	tmp = ft_strjoin(result, char_str);
	free(result);
	free(char_str);
	return (tmp);
}

char	*expand_double_quotes(char *input, t_env *env)
{
	char	*result;
	int		i;

	result = ft_strdup("");
	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
			result = expand_var_in_quotes(result, input, &i, env);
		else
			result = append_character(result, input[i]);
		i++;
	}
	return (result);
}

char	*expand_token(char *input, t_env *env)
{
	char	*inner;
	char	*expanded;

	if (input[0] == '\'' && input[ft_strlen(input) - 1] == '\'')
		return (ft_substr(input, 1, ft_strlen(input) - 2));
	if (input[0] == '"' && input[ft_strlen(input) - 1] == '"')
	{
		inner = ft_substr(input, 1, ft_strlen(input) - 2);
		expanded = expand_double_quotes(inner, env);
		free(inner);
		return (expanded);
	}
	return (expand_variable(input, env));
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
