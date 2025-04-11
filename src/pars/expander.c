/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 09:30:34 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/12 01:35:35 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*append_char_to_result(char *result, char *tmp)
{
	char	*new_result;

	new_result = ft_strjoin_free(result, tmp);
	free(tmp);
	if (!new_result)
		return (free(result), NULL);
	return (new_result);
}

char	*process_dollar_in_quotes(char *input, int *i,
	t_data *data, char *result)
{
	char	*tmp;
	int		len;

	if (input[*i] == '$' && (ft_isalnum(input[*i + 1]) || input[*i + 1] == '_'))
		tmp = expand_variable(&input[*i], data->env_list, &len);
	else if (input[*i] == '$' && input[*i + 1] == '?')
		tmp = ft_itoa(data->exit_status);
	else if (input[*i] == '$' && (!input[*i + 1] || input[*i + 1] == ' ' ||
		input[*i + 1] == '\t' || input[*i + 1] == '\n' || input[*i + 1] == '"'))
	{
		tmp = ft_strdup("$");
		if (!tmp)
			return (free(result), NULL);
		len = 1;
	}
	else
		tmp = ft_substr(input, (*i)++, 1);
	if (!tmp)
		return (free(result), NULL);
	result = append_char_to_result(result, tmp);
	if (!result)
		return (NULL);
	*i += len;
	return (result);
}

char	*process_double_quotes_content(char *input, int *i,
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

char	*process_single_quotes_content(char *input, int *i, char *result)
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
