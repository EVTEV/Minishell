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

	while (input[*start] && input[*start] == ' ')
		(*start)++;
	i = *start;
	while (input[i] && input[i] != ' ')
		i++;
	len = i - *start;
	if (len <= 0)
		return (NULL);
	*start = i; // Update the start index to the next position after the word
	return (ft_substr(input, *start - len, len));
}

t_token	*lexer(char *input)
{
	t_token	*head;
	char	*current_word;
	int		start;

	head = NULL;
	start = 0;
	while ((current_word = get_next_word(input, &start)))
	{
		add_token(&head, create_token(current_word, WORD));
		free(current_word);
		start += ft_strlen(current_word);
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
