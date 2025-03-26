/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:31:38 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 22:34:20 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	*append_and_free(char *s1, char *s2)
{
	char	*new;

	new = ft_strjoin(s1, s2);
	if (!new)
		return (NULL);
	free(s1);
	return (new);
}

char	*get_var_name(const char *s, int *i)
{
	int		start;
	int		len;
	char	*name;

	if (s[*i] == '$')
		(*i)++;
	if (s[*i] == '?')
	{
		(*i)++;
		name = ft_strdup("?");
		if (!name)
			return (NULL);
		return (name);
	}
	start = *i;
	while (s[*i] && (ft_isalnum(s[*i]) || s[*i] == '_'))
		(*i)++;
	len = *i - start;
	name = ft_substr(s, start, len);
	if (!name)
		return (NULL);
	return (name);
}

char	*get_var_value(char *name, t_env *env, int status)
{
	char	*res;
	char	*tmp;

	if (!ft_strncmp(name, "?", 1))
	{
		tmp = ft_itoa(status);
		if (!tmp)
			return (NULL);
		res = ft_strdup(tmp);
		if (!res)
			return (free(tmp), NULL);
		return (free(tmp), free(name), res);
	}
	while (env)
	{
		if (!ft_strncmp(env->key, name, ft_strlen(name)))
		{
			res = ft_strdup(env->value);
			if (!res)
				return (NULL);
			return (free(name), res);
		}
		env = env->next;
	}
	return (free(name), ft_strdup(""));
}

int	expand_tokens(t_token *tokens, t_env *env, int last_status)
{
	char	*expanded;

	while (tokens)
	{
		if (tokens->type == WORD && tokens->value)
		{
			expanded = expand_string(tokens->value, env, last_status);
			if (!expanded)
				return (0);
			free(tokens->value);
			tokens->value = expanded;
		}
		tokens = tokens->next;
	}
	return (1);
}

int	is_var_start(char c)
{
	return (ft_isalnum(c) || c == '_' || c == '?');
}
