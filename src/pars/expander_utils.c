/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:25:20 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/12 01:31:35 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*expand_dollar_variable(char *input, int *i,
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

char	*expand_variable(char *input, t_env *env_list, int *len)
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
