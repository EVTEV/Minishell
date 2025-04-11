/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:18:49 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/11 19:18:54 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*process_quoted_part(char *value, size_t *i,
	size_t len, char *processed)
{
	char	quote;
	size_t	start;
	char	*quoted_part;

	quote = value[(*i)++];
	start = *i;
	while (*i < len && value[*i] != quote)
		(*i)++;
	quoted_part = ft_substr(value, start, *i - start);
	if (!quoted_part)
	{
		free(processed);
		return (NULL);
	}
	processed = concatenate_parts(processed, quoted_part);
	if (!processed)
		return (NULL);
	(*i)++;
	return (processed);
}

char	*process_unquoted_part(char *value,
	size_t *i, size_t len, char *processed)
{
	size_t	start;
	char	*unquoted_part;

	unquoted_part = NULL;
	start = *i;
	while (*i < len && value[*i] != '\'' && value[*i] != '\"')
		(*i)++;
	unquoted_part = ft_substr(value, start, *i - start);
	if (!unquoted_part)
	{
		free(processed);
		return (NULL);
	}
	processed = concatenate_parts(processed, unquoted_part);
	if (!processed)
		return (NULL);
	return (processed);
}

char	*process_value(char *value)
{
	char	*processed;
	size_t	i;
	size_t	len;

	len = ft_strlen(value);
	i = 0;
	processed = NULL;
	while (i < len)
	{
		if (value[i] == '\'' || value[i] == '\"')
			processed = process_quoted_part(value, &i, len, processed);
		else
			processed = process_unquoted_part(value, &i, len, processed);
		if (!processed)
			return (free(value), value = NULL, NULL);
	}
	free(value);
	value = NULL;
	return (processed);
}

t_token	*create_new_token(char *value, int type)
{
	t_token	*new;

	new = (t_token *)malloc(sizeof(t_token));
	if (!new)
	{
		free(value);
		value = NULL;
		return (NULL);
	}
	new->value = ft_strdup(value);
	if (!new->value)
	{
		free(new);
		return (NULL);
	}
	new->type = type;
	new->next = NULL;
	return (new);
}

void	append_token(t_token **tokens, t_token *new)
{
	t_token	*tmp;

	if (!*tokens)
		*tokens = new;
	else
	{
		tmp = *tokens;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
	}
}
