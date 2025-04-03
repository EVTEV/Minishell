/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:52:23 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/03 17:52:31 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*expand_var_in_quo(char *result, char *input, int *i, t_env *env)
{
	int		start;
	char	*var_name;
	char	*var_result;

	start = *i + 1;
	while (input[start] && is_valid_env_char(input[start], start == *i + 1))
		start++;
	var_name = ft_substr(input, *i + 1, start - (*i + 1));
	var_result = get_env_value(env, var_name);
	free(var_name);
	if (var_result)
	{
		result = ft_strjoin_free(result, var_result);
		*i = start - 1;
	}
	else
		*i = start - 1;
	return (result);
}

int	is_valid_env_char(char c, int is_first)
{
	if (is_first)
		return (ft_isalpha(c) || c == '_');
	return (ft_isalnum(c) || c == '_');
}

char	*expand_env_variable(char *input, t_env *env)
{
	char	*expanded;
	char	*value;

	if (!input || !*input)
		return (ft_strdup(""));
	if (input[1] == '"' || input[1] == '\'' || input[1] == '\\')
		return (ft_strdup(input));
	if (ft_isdigit(input[1]))
		return (ft_strdup(input + 2));
	if (!is_valid_env_char(input[1], 1))
		return (ft_strdup(input + 1));
	value = get_env_value(env, input + 1);
	if (!value)
		return (ft_strdup(""));
	expanded = ft_strdup(value);
	return (expanded);
}

char	*expand_variable(char *input, t_env *env)
{
	char	*value;

	if (!input || !*input || input[0] != '$')
		return (ft_strdup(input));
	if (ft_isdigit(input[1]))
		return (ft_strdup(input + 2));
	value = get_env_value(env, input + 1);
	if (!value)
		return (ft_strdup(""));
	return (ft_strdup(value));
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
			result = expand_var_in_quo(result, input, &i, env);
		else
			result = append_character(result, input[i]);
		i++;
	}
	return (result);
}
