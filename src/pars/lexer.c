/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:35:20 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 21:07:51 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	is_op_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static char	*extract_word(const char *s, int *i)
{
	int		start;
	int		len;
	char	*res;

	start = *i;
	while (s[*i] && !ft_isspace(s[*i]) && !is_op_char(s[*i])
		&& s[*i] != '"' && s[*i] != '\'')
		(*i)++;
	len = *i - start;
	res = ft_strndup(s + start, len);
	if (!res)
		return (NULL);
	return (res);
}

static char	*collect_full_word(const char *s, int *i)
{
	char	*res;
	char	*part;
	char	*tmp;

	res = ft_strdup("");
	if (!res)
		return (NULL);
	while (s[*i] && !ft_isspace(s[*i]) && !is_op_char(s[*i]))
	{
		if (s[*i] == '\'' || s[*i] == '"')
			part = extract_quoted(s, i, s[*i]);
		else
			part = extract_word(s, i);
		if (!part)
			return (free(res), NULL);
		tmp = res;
		res = ft_strjoin(tmp, part);
		if (!res)
			return (free(tmp), free(part), NULL);
		free(tmp);
		free(part);
	}
	return (res);
}

static void	handle_operator(const char *s, int *i, t_token **lst)
{
	char	op[3];

	if (s[*i] == '|')
	{
		op[0] = '|';
		op[1] = '\0';
		add_operator_token(lst, op);
		(*i)++;
		return ;
	}
	op[0] = s[*i];
	op[1] = '\0';
	op[2] = '\0';
	if (s[*i + 1] == s[*i])
	{
		op[1] = s[*i];
		(*i)++;
	}
	(*i)++;
	add_operator_token(lst, op);
}

t_token	*tokenize(char *line)
{
	t_token	*lst;
	char	*word;
	int		i;

	i = 0;
	lst = NULL;
	while (line[i])
	{
		while (ft_isspace(line[i]))
			i++;
		if (!line)
			break ;
		if (is_op_char(line[i]))
			handle_operator(line, &i, &lst);
		else
		{
			word = collect_full_word(line, &i);
			if (!word)
				return (ft_printf("%s", QUOTE_ERR), free(lst), NULL);
			add_token(&lst, create_token(word, WORD));
		}
	}
	return (lst);
}
