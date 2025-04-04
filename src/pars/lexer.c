#include "../../inc/minishell.h"

# define TOKEN_REDIR_OUT 3
# define TOKEN_REDIR_APPEND 4
# define TOKEN_REDIR_IN 5

static void	add_token(t_token **tokens, char *value, int type)
{
	t_token	*new;
	t_token	*tmp;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	new->value = value;
	new->type = type;
	new->next = NULL;
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

static int	handle_quotes(char *input, int i, char quote)
{
	i++;
	while (input[i] && input[i] != quote)
		i++;
	return (i);
}

t_token	*lexer(char *input)
{
	t_token	*tokens = NULL;
	int		i = 0;
	int		start;

	while (input && input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
			i++;
		else if (input[i] == '\'' || input[i] == '\"')
		{
			start = i++;
			i = handle_quotes(input, i, input[start]);
			add_token(&tokens, ft_substr(input, start, i - start + 1), TOKEN_WORD);
			i++;
		}
		else if (input[i] == '|')
		{
			add_token(&tokens, ft_strdup("|"), TOKEN_PIPE);
			i++;
		}
		else if (input[i] == '>' && input[i + 1] == '>')
		{
			add_token(&tokens, ft_strdup(">>"), TOKEN_REDIR_APPEND);
			i += 2;
		}
		else if (input[i] == '>')
		{
			add_token(&tokens, ft_strdup(">"), TOKEN_REDIR_OUT);
			i++;
		}
		else if (input[i] == '<')
		{
			add_token(&tokens, ft_strdup("<"), TOKEN_REDIR_IN);
			i++;
		}
		else
		{
			start = i;
			while (input[i] && input[i] != ' ' && input[i] != '\t' && input[i] != '\'' && input[i] != '\"' && input[i] != '|' && input[i] != '>' && input[i] != '<')
				i++;
			add_token(&tokens, ft_substr(input, start, i - start), TOKEN_WORD);
		}
	}
	return (tokens);
}
