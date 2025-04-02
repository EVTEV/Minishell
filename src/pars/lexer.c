/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/02 18:31:01 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/02 19:40:53 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*create_token(char *value, t_token_type type)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
	if (!token)
		return (NULL);
	token->value = ft_strdup(value);
	if (!token->value)
	{
		free(token);
		return (NULL);
	}
	token->type = type;
	token->next = NULL;
	return (token);
}

void	add_token(t_token **head, t_token *new_token)
{
	t_token	*current;

	if (!*head)
	{
		*head = new_token;
		return ;
	}
	current = *head;
	while (current->next)
		current = current->next;
	current->next = new_token;
}

char	*get_next_word(char *input, int *start)
{
	int		i;
	int		len;
	char	quote;

	if (!input || !*input)
		return (NULL);
	while (input[*start] && input[*start] == ' ')
		(*start)++;
	i = *start;
	quote = 0;
	while (input[i] && (quote || input[i] != ' '))
	{
		if (!quote && (input[i] == '\'' || input[i] == '"'))
			quote = input[i];
		else if (quote && input[i] == quote)
			quote = 0;
		i++;
	}
	len = i - *start;
	if (len <= 0)
		return (NULL);
	*start = i;
	return (ft_substr(input, *start - len, len));
}

t_token	*lexer(char *input)
{
	t_token	*head;
	t_token	*new_token;
	char	*current_word;
	int		start;

	head = NULL;
	start = 0;
	current_word = get_next_word(input, &start);
	while (current_word)
	{
		new_token = create_token(current_word, WORD);
		add_token(&head, new_token);
		free(current_word);
		current_word = get_next_word(input, &start);
	}
	return (head);
}

void	free_tokens(t_token *head)
{
	t_token	*tmp;

	if (!head)
		return ;
	while (head)
	{
		tmp = head;
		head = head->next;
		if (tmp->value)
		{
			free(tmp->value);
			tmp->value = NULL;
		}
		if (tmp)
		{
			free(tmp);
			tmp = NULL;
		}
	}
}
