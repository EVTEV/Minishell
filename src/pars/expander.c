/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 09:30:34 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/11 13:29:54 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*expand_variable(char *input, t_env *env_list, int *len)
{
	char	*name;
	char	*value;
	char	*result;
	int		i;

	i = 1;
	while (input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
		i++;
	*len = i;
	name = ft_substr(input, 1, i - 1);
	if (!name)
		return (NULL);
	if (!*name)
	{
		free(name);
		return (ft_strdup("$"));
	}
	value = get_value(*env_list, name);
	free(name);
	if (!value)
		return (ft_strdup(""));
	result = ft_strdup(value);
	if (!result)
		return (NULL);
	return (result);
}

static char	*append_char_to_result(char *result, char *tmp)
{
	char	*new_result;

	new_result = ft_strjoin_free(result, tmp);
	free(tmp);
	if (!new_result)
		return (free(result), NULL);
	return (new_result);
}

static char	*process_dollar_in_quotes(char *input, int *i,
	t_data *data, char *result)
{
	char	*tmp;
	int		len;

	if (input[*i] == '$' && (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_'))
		tmp = expand_variable(&input[*i], data->env_list, &len);
	else if (input[*i] == '$' && input[*i + 1] == '?')
		tmp = ft_itoa(data->exit_status);
	else
		tmp = ft_substr(input, (*i)++, 1);
	if (!tmp)
		return (free(result), NULL);
	result = append_char_to_result(result, tmp);
	if (!result)
		return (NULL);
	if (input[*i] == '$' && input[*i + 1] == '?')
		*i += 2;
	else
		*i += len;
	return (result);
}

static char	*process_double_quotes_content(char *input, int *i,
	t_data *data, char *result)
{
	char	*tmp;
	char	*tmpp;

	while (input[*i] && input[*i] != '"')
	{
		if (input[*i] == '$')
			result = process_dollar_in_quotes(input, i, data, result);
		else
		{
			tmp = ft_substr(input, (*i)++, 1);
			if (!tmp)
				return (free(result), NULL);
			tmpp = ft_strjoin_free(result, tmp);
			free(tmp);
			if (!tmpp)
				return (free(result), NULL);
			result = ft_strdup(tmpp);
			free(tmpp);
			if (!result)
				return (NULL);
		}
	}
	return (result);
}

static char	*handle_double_quotes(char *input, int *i,
	t_data *data, char *result)
{
	char	*tmp;

	tmp = ft_substr(input, (*i)++, 1);
	if (!tmp)
		return (free(result), NULL);
	result = append_char_to_result(result, tmp);
	if (!result)
		return (NULL);
	result = process_double_quotes_content(input, i, data, result);
	if (!result)
		return (NULL);
	if (!input[*i])
	{
		ft_putstr_fd("minishell: syntax error: unclosed double quote\n",
			STDERR_FILENO);
		return (free(result), NULL);
	}
	tmp = ft_substr(input, (*i)++, 1);
	if (!tmp)
		return (free(result), NULL);
	return (append_char_to_result(result, tmp));
}

static char	*process_single_quotes_content(char *input, int *i, char *result)
{
	char	*tmp;
	int		start;

	start = *i;
	while (input[*i] && input[*i] != '\'')
		(*i)++;
	if (!input[*i])
	{
		ft_putstr_fd("minishell: syntax error: unclosed single quote\n",
			STDERR_FILENO);
		return (free(result), NULL);
	}
	tmp = ft_substr(input, start, *i - start);
	if (!tmp)
		return (free(result), NULL);
	result = append_char_to_result(result, tmp);
	if (!result)
		return (NULL);
	return (result);
}

static char	*handle_single_quotes(char *input, int *i, char *result)
{
	char	*tmp;

	tmp = ft_substr(input, (*i)++, 1);
	if (!tmp)
		return (free(result), NULL);
	result = append_char_to_result(result, tmp);
	if (!result)
		return (NULL);
	result = process_single_quotes_content(input, i, result);
	if (!result)
		return (NULL);
	tmp = ft_substr(input, (*i)++, 1);
	if (!tmp)
		return (free(result), NULL);
	return (append_char_to_result(result, tmp));
}

static char	*handle_empty_dollar(char *result)
{
	char	*tmp;

	tmp = ft_strdup("$");
	if (!tmp)
		return (free(result), NULL);
	result = ft_strjoin_free(result, tmp);
	free(tmp);
	if (!result)
		return (NULL);
	return (result);
}

static char	*expand_dollar_variable(char *input, int *i,
	t_data *data, char *result)
{
	char	*tmp;
	int		len;

	if (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_')
		tmp = expand_variable(&input[*i], data->env_list, &len);
	else if (input[*i + 1] == '?')
		tmp = ft_itoa(data->exit_status);
	else
		tmp = ft_strdup("$");
	if (!tmp)
		return (free(result), NULL);
	result = ft_strjoin_free(result, tmp);
	free(tmp);
	if (!result)
		return (NULL);
	if (input[*i + 1] == '?')
		*i += 2;
	else
		*i += len;
	return (result);
}

static char	*handle_dollar_sign(char *input, int *i, t_data *data, char *result)
{
	if (!input[*i + 1])
	{
		result = handle_empty_dollar(result);
		if (!result)
			return (NULL);
		(*i)++;
		return (result);
	}
	return (expand_dollar_variable(input, i, data, result));
}

static char	*handle_plain_text(char *input, int *i, char *result)
{
	char	*tmp;
	int		start;

	start = *i;
	while (input[*i] && input[*i] != '$'
		&& input[*i] != '"' && input[*i] != '\'')
		(*i)++;
	tmp = ft_substr(input, start, *i - start);
	if (!tmp)
		return (free(result), NULL);
	result = ft_strjoin_free(result, tmp);
	free(tmp);
	if (!result)
		return (NULL);
	return (result);
}

char	*expander(char *input, t_data *data)
{
	char	*result;
	int		i;

	result = NULL;
	i = 0;
	while (input[i])
	{
		if (input[i] == '"')
			result = handle_double_quotes(input, &i, data, result);
		else if (input[i] == '\'')
			result = handle_single_quotes(input, &i, result);
		else if (input[i] == '$')
			result = handle_dollar_sign(input, &i, data, result);
		else
			result = handle_plain_text(input, &i, result);
		if (!result)
			return (NULL);
	}
	return (result);
}
