/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 22:03:40 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 22:10:29 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	add_redir(t_redir **list, t_redir *new)
{
	t_redir	*tmp;

	if (!*list)
	{
		*list = new;
		return ;
	}
	tmp = *list;
	while (tmp->next)
		tmp = tmp->next;
	tmp->next = new;
}

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type != PIPE)
	{
		if (token->type == WORD)
			count++;
		else if (token->type >= REDIR_IN && token->type <= HEREDOC)
			token = token->next;
		token = token->next;
	}
	return (count);
}

char	**fill_args(t_token **tok, t_env *env, int status, int count)
{
	char	**args;
	int		i;

	i = 0;
	args = (char **)malloc(sizeof(char *) * (count + 1));
	if (!args)
		return (NULL);
	while (*tok && (*tok)->type != PIPE)
	{
		if ((*tok)->type == WORD)
		{
			args[i] = expand_string((*tok)->value, env, status);
			if (!args[i])
				return (free_args(args), NULL);
			i++;
		}
		else if ((*tok)->type >= REDIR_IN && (*tok)->type <= HEREDOC)
			*tok = (*tok)->next;
		*tok = (*tok)->next;
	}
	args[i] = NULL;
	return (args);
}

char	**extract_args(t_token **tok, t_env *env, int status)
{
	int	count;

	count = count_args(*tok);
	return (fill_args(tok, env, status, count));
}

t_redir	*extract_redirs(t_token **tok)
{
	t_redir			*list;
	t_redir			*new;
	char			*file;
	t_token_type	type;

	list = NULL;
	while (*tok && (*tok)->type != PIPE)
	{
		if ((*tok)->type >= REDIR_IN && (*tok)->type <= HEREDOC)
		{
			type = (*tok)->type;
			*tok = (*tok)->next;
			if (!*tok || (*tok)->type != WORD)
				return (NULL);
			file = ft_strdup((*tok)->value);
			if (!file)
				return (NULL);
			new = create_redir(file, type);
			if (!new)
				return (NULL);
			add_redir(&list, new);
		}
		*tok = (*tok)->next;
	}
	return (list);
}
