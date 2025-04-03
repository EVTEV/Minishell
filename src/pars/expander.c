/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:31:03 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/03 17:53:49 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*append_character(char *result, char c)
{
	char	*tmp;
	char	*char_str;

	char_str = ft_substr(&c, 0, 1);
	tmp = ft_strjoin(result, char_str);
	free(result);
	free(char_str);
	return (tmp);
}

static char	*handle_single_quotes(char *input)
{
	return (ft_substr(input, 1, ft_strlen(input) - 2));
}

static char	*handle_double_quotes(char *input, t_env *env)
{
	char	*inner;
	char	*expanded;

	inner = ft_substr(input, 1, ft_strlen(input) - 2);
	expanded = expand_double_quotes(inner, env);
	free(inner);
	return (expanded);
}

static char	*expand_unquoted_token(char *input, t_env *env)
{
	char	*expanded;
	int		i;

	expanded = ft_strdup("");
	i = 0;
	while (input[i])
	{
		if (input[i] == '$')
			expanded = expand_var_in_quo(expanded, input, &i, env);
		else
			expanded = append_character(expanded, input[i]);
		i++;
	}
	return (expanded);
}

char	*expand_token(char *input, t_env *env)
{
	if (input[0] == '\'' && input[ft_strlen(input) - 1] == '\'')
		return (handle_single_quotes(input));
	if (input[0] == '"' && input[ft_strlen(input) - 1] == '"')
		return (handle_double_quotes(input, env));
	return (expand_unquoted_token(input, env));
}
