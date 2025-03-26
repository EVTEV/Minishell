/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extract_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:30:16 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 20:58:53 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	add_operator_token(t_token **lst, char *op)
{
	t_token_type	type;
	char			*dup;

	dup = ft_strdup(op);
	if (!dup)
		return ;
	if (!ft_strncmp(op, ">>", 2))
		type = REDIR_APPEND;
	else if (!ft_strncmp(op, "<<", 2))
		type = HEREDOC;
	else if (!ft_strncmp(op, "<", 1))
		type = REDIR_IN;
	else if (!ft_strncmp(op, ">", 1))
		type = REDIR_OUT;
	else
		type = PIPE;
	add_token(lst, create_token(dup, type));
}

char	*ft_strndup(const char *s, size_t n)
{
	char	*cpy;
	size_t	i;

	cpy = (char *)malloc(sizeof(char) * (n + 1));
	if (!cpy)
		return (NULL);
	i = 0;
	while (i < n && s[i])
	{
		cpy[i] = s[i];
		i++;
	}
	cpy[i] = '\0';
	return (cpy);
}

char	*extract_quoted(const char *s, int *i, char quote)
{
	int		start;
	int		len;
	char	*result;

	(*i)++;
	start = *i;
	while (s[*i] && s[*i] != quote)
		(*i)++;
	if (s[*i] != quote)
		return (NULL);
	len = *i - start;
	result = ft_strndup(s + start, len);
	if (!result)
		return (NULL);
	(*i)++;
	return (result);
}
