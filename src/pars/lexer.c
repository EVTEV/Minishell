#include "../../inc/minishell.h"

// Forward declaration for concatenate_parts
static char	*concatenate_parts(char *part1, char *part2);

static void	add_token(t_token **tokens, char *value, int type)
{
	t_token	*new;
	t_token	*tmp;

	// Remove quotes for TOKEN_WORD and handle concatenation of quoted parts
	if (type == TOKEN_WORD && value)
	{
		char *processed = NULL;
		size_t i = 0, len = ft_strlen(value);

		while (i < len)
		{
			if (value[i] == '\'' || value[i] == '\"')
			{
				char quote = value[i++];
				size_t start = i;
				while (i < len && value[i] != quote)
					i++;
				char *quoted_part = ft_substr(value, start, i - start);
				processed = concatenate_parts(processed, quoted_part);
				i++; // Skip closing quote
			}
			else
			{
				size_t start = i;
				while (i < len && value[i] != '\'' && value[i] != '\"')
					i++;
				char *unquoted_part = ft_substr(value, start, i - start);
				processed = concatenate_parts(processed, unquoted_part);
			}
		}
		free(value);
		value = processed;
	}

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
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i]) // If the quote is not closed
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n", STDERR_FILENO);
		return (-1); // Return -1 to indicate an error
	}
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
		if (input[i] == '\\' && input[i + 1]) // Handle escaped characters
		{
			start = ++i; // Skip the backslash
			i++;
			char *escaped_part = ft_substr(input, start, 1); // Extract the escaped character
			current_part = concatenate_parts(current_part, escaped_part);
		}
		else if (input[i] == ' ' || input[i] == '\t')
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
			if (i == -1) // If an error occurred due to unclosed quotes
			{
				free(current_part); // Free any partially constructed token
				free_token(tokens); // Free the token list
				return (NULL);
			}
			char *quoted_part = ft_substr(input, start, i - start + 1); // Include quotes
			current_part = concatenate_parts(current_part, quoted_part);
			i++;
		}
		else if ((input[i] == '|' || input[i] == '<' || input[i] == '>') && 
		         (!current_part || current_part[0] != '\'' || current_part[0] != '\"'))
		{
			if (current_part)
			{
				add_token(&tokens, current_part, TOKEN_WORD);
				current_part = NULL;
			}
			if (input[i] == '>' && input[i + 1] == '>')
			{
				add_token(&tokens, ft_strdup(">>"), TOKEN_REDIR_APPEND);
				i += 2;
			}
			else if (input[i] == '<' && input[i + 1] == '<')
			{
				add_token(&tokens, ft_strdup("<<"), TOKEN_REDIR_HEREDOC);
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
			else if (input[i] == '|')
			{
				add_token(&tokens, ft_strdup("|"), TOKEN_PIPE);
				i++;
			}
		}
		else
		{
			start = i;
			while (input[i] && input[i] != ' ' && input[i] != '\t' && 
			       input[i] != '\'' && input[i] != '\"' && 
			       input[i] != '|' && input[i] != '>' && input[i] != '<')
				i++;
			char *unquoted_part = ft_substr(input, start, i - start);
			current_part = concatenate_parts(current_part, unquoted_part);
		}
	}
	if (current_part)
		add_token(&tokens, current_part, TOKEN_WORD);
	return (tokens);
}
