#include "../../inc/minishell.h"

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

static char	*concatenate_parts(char *part1, char *part2)
{
	char	*result;

	if (!part1)
		return (part2);
	if (!part2)
		return (part1);
	result = ft_strjoin(part1, part2);
	free(part1);
	free(part2);
	return (result);
}

t_token	*lexer(char *input)
{
	t_token	*tokens = NULL;
	int		i = 0;
	int		start;
	char	*current_part = NULL;

	while (input && input[i])
	{
		if (input[i] == ' ' || input[i] == '\t')
		{
			if (current_part)
			{
				add_token(&tokens, current_part, TOKEN_WORD);
				current_part = NULL;
			}
			i++;
		}
		else if (input[i] == '\'' || input[i] == '\"')
		{
			start = i++;
			i = handle_quotes(input, i, input[start]);
			char *quoted_part = ft_substr(input, start + 1, i - start - 1); // Exclude quotes
			current_part = concatenate_parts(current_part, quoted_part);
			i++;
		}
		else if (input[i] == '|')
		{
			if (current_part)
			{
				add_token(&tokens, current_part, TOKEN_WORD);
				current_part = NULL;
			}
			add_token(&tokens, ft_strdup("|"), TOKEN_PIPE);
			i++;
		}
		else if (input[i] == '>' && input[i + 1] == '>')
		{
			if (current_part)
			{
				add_token(&tokens, current_part, TOKEN_WORD);
				current_part = NULL;
			}
			add_token(&tokens, ft_strdup(">>"), TOKEN_REDIR_APPEND);
			i += 2;
		}
		else if (input[i] == '>')
		{
			if (current_part)
			{
				add_token(&tokens, current_part, TOKEN_WORD);
				current_part = NULL;
			}
			add_token(&tokens, ft_strdup(">"), TOKEN_REDIR_OUT);
			i++;
		}
		else if (input[i] == '<' && input[i + 1] == '<')
		{
			if (current_part)
			{
				add_token(&tokens, current_part, TOKEN_WORD);
				current_part = NULL;
			}
			add_token(&tokens, ft_strdup("<<"), TOKEN_REDIR_HEREDOC);
			i += 2;
		}
		else if (input[i] == '<')
		{
			if (current_part)
			{
				add_token(&tokens, current_part, TOKEN_WORD);
				current_part = NULL;
			}
			add_token(&tokens, ft_strdup("<"), TOKEN_REDIR_IN);
			i++;
		}
		else
		{
			start = i;
			while (input[i] && input[i] != ' ' && input[i] != '\t' && input[i] != '\'' && input[i] != '\"' && input[i] != '|' && input[i] != '>' && input[i] != '<')
				i++;
			char *unquoted_part = ft_substr(input, start, i - start);
			current_part = concatenate_parts(current_part, unquoted_part);
		}
	}
	if (current_part)
		add_token(&tokens, current_part, TOKEN_WORD);
	return (tokens);
}
