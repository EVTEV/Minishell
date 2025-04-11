/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 14:28:49 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/11 18:46:23 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// Forward declaration for concatenate_parts
static char	*concatenate_parts(char *part1, char *part2);

static char	*process_quoted_part(char *value, size_t *i,
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

static char	*process_unquoted_part(char *value,
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

static char	*process_value(char *value)
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

static t_token	*create_new_token(char *value, int type)
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

static void	append_token(t_token **tokens, t_token *new)
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

static void	add_token(t_token **tokens, char *value, int type)
{
	t_token	*new;

	new = NULL;
	if (type == TOKEN_WORD && value)
	{
		value = process_value(value);
		if (!value)
			return ;
	}
	new = create_new_token(value, type);
	if (!new)
	{
		free(value);
		value = NULL;
		return ;
	}
	append_token(tokens, new);
}

static int	handle_quotes(char *input, int i, char quote)
{
	while (input[i] && input[i] != quote)
		i++;
	if (!input[i])
	{
		ft_putstr_fd("minishell: syntax error: unclosed quote\n",
			STDERR_FILENO);
		return (-1);
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
	if (!result)
	{
		free(part1);
		free(part2);
		return (NULL);
	}
	free(part1);
	free(part2);
	return (result);
}

static int	handle_escaped_characters(char *input, int *i,
	char **current_part, t_token **tokens)
{
	int		start;
	char	*escaped_part;

	start = ++(*i);
	(*i)++;
	escaped_part = ft_substr(input, start, 1);
	if (!escaped_part)
		return (free(*current_part), free_token(*tokens), -1);
	*current_part = concatenate_parts(*current_part, escaped_part);
	if (!*current_part)
		return (free_token(*tokens), -1);
	return (0);
}

static int	handle_whitespace(char **current_part, t_token **tokens)
{
	if (*current_part)
	{
		add_token(tokens, *current_part, TOKEN_WORD);
		if (!*tokens)
			return (free(*current_part), free_token(*tokens), -1);
		*current_part = NULL;
	}
	return (0);
}

static int	handle_quotes_in_lexer(char *input, int *i,
	char **current_part, t_token **tokens)
{
	int		start;
	char	*quoted_part;

	start = (*i)++;
	*i = handle_quotes(input, *i, input[start]);
	if (*i == -1)
		return (free(*current_part), free_token(*tokens), -1);
	quoted_part = ft_substr(input, start, *i - start + 1);
	if (!quoted_part)
		return (free(*current_part), free_token(*tokens), -1);
	*current_part = concatenate_parts(*current_part, quoted_part);
	if (!*current_part)
		return (free_token(*tokens), -1);
	(*i)++;
	return (0);
}

static int	handle_redirection_append(int *i, t_token **tokens)
{
	char	*append_redir;

	append_redir = ft_strdup(">>");
	if (!append_redir)
		return (free_token(*tokens), -1);
	add_token(tokens, append_redir, TOKEN_REDIR_APPEND);
	if (!*tokens)
		return (free_token(*tokens), -1);
	*i += 2;
	return (0);
}

static int	handle_redirection_heredoc(int *i, t_token **tokens)
{
	char	*heredoc_redir;

	heredoc_redir = ft_strdup("<<");
	if (!heredoc_redir)
		return (free_token(*tokens), -1);
	add_token(tokens, heredoc_redir, TOKEN_REDIR_HEREDOC);
	if (!*tokens)
		return (free_token(*tokens), -1);
	*i += 2;
	return (0);
}

static int	handle_redirection_out(int *i, t_token **tokens)
{
	char	*out_redir;

	out_redir = ft_strdup(">");
	if (!out_redir)
		return (free_token(*tokens), -1);
	add_token(tokens, out_redir, TOKEN_REDIR_OUT);
	if (!*tokens)
		return (free_token(*tokens), -1);
	(*i)++;
	return (0);
}

static int	handle_redirection_in(int *i, t_token **tokens)
{
	char	*in_redir;

	in_redir = ft_strdup("<");
	if (!in_redir)
		return (free_token(*tokens), -1);
	add_token(tokens, in_redir, TOKEN_REDIR_IN);
	if (!*tokens)
		return (free_token(*tokens), -1);
	(*i)++;
	return (0);
}

static int	handle_redirections(char *input, int *i, t_token **tokens)
{
	if (input[*i] == '>' && input[*i + 1] == '>')
		return (handle_redirection_append(i, tokens));
	else if (input[*i] == '<' && input[*i + 1] == '<')
		return (handle_redirection_heredoc(i, tokens));
	else if (input[*i] == '>')
		return (handle_redirection_out(i, tokens));
	else if (input[*i] == '<')
		return (handle_redirection_in(i, tokens));
	return (0);
}

static int	handle_pipe(char *input, int *i, t_token **tokens)
{
	char	*pipe_token;

	pipe_token = NULL;
	if (input[*i] == '|')
	{
		pipe_token = ft_strdup("|");
		if (!pipe_token)
			return (free_token(*tokens), -1);
		add_token(tokens, pipe_token, TOKEN_PIPE);
		if (!*tokens)
			return (free_token(*tokens), -1);
		(*i)++;
	}
	return (0);
}

static int	handle_special_characters(char *input, int *i,
	char **current_part, t_token **tokens)
{
	if (*current_part)
	{
		add_token(tokens, *current_part, TOKEN_WORD);
		if (!*tokens)
			return (free(*current_part), free_token(*tokens), -1);
		*current_part = NULL;
	}
	if (handle_redirections(input, i, tokens) == -1)
		return (-1);
	if (handle_pipe(input, i, tokens) == -1)
		return (-1);
	return (0);
}

static int	handle_unquoted_part(char *input, int *i,
	char **current_part, t_token **tokens)
{
	int		start;
	char	*unquoted_part;

	start = *i;
	while (input[*i] && input[*i] != ' ' && input[*i] != '\t'
		&& input[*i] != '\'' && input[*i] != '\"'
		&& input[*i] != '|' && input[*i] != '>' && input[*i] != '<')
		(*i)++;
	unquoted_part = ft_substr(input, start, *i - start);
	if (!unquoted_part)
		return (free(*current_part), free_token(*tokens), -1);
	*current_part = concatenate_parts(*current_part, unquoted_part);
	if (!*current_part)
		return (free_token(*tokens), -1);
	return (0);
}

static int	handle_escape_case(char *input, int *i,
	char **current_part, t_token **tokens)
{
	if (input[*i] == '\\' && input[*i + 1])
		return (handle_escaped_characters(input, i, current_part, tokens));
	return (0);
}

static int	handle_whitespace_case(char *input, int *i,
	char **current_part, t_token **tokens)
{
	if (input[*i] == ' ' || input[*i] == '\t')
	{
		if (handle_whitespace(current_part, tokens) == -1)
			return (-1);
		(*i)++;
	}
	return (0);
}

static int	handle_quote_case(char *input, int *i,
	char **current_part, t_token **tokens)
{
	if (input[*i] == '\'' || input[*i] == '\"')
		return (handle_quotes_in_lexer(input, i, current_part, tokens));
	return (0);
}

static int	handle_special_char_case(char *input, int *i,
	char **current_part, t_token **tokens)
{
	if (input[*i] == '|' || input[*i] == '<' || input[*i] == '>')
		return (handle_special_characters(input, i, current_part, tokens));
	return (0);
}

static int	handle_default_case(char *input, int *i,
	char **current_part, t_token **tokens)
{
	return (handle_unquoted_part(input, i, current_part, tokens));
}

static int	process_input_char(char *input, int *i,
	char **current_part, t_token **tokens)
{
	if (handle_escape_case(input, i, current_part, tokens) == -1)
		return (-1);
	if (handle_whitespace_case(input, i, current_part, tokens) == -1)
		return (-1);
	if (handle_quote_case(input, i, current_part, tokens) == -1)
		return (-1);
	if (handle_special_char_case(input, i, current_part, tokens) == -1)
		return (-1);
	if (handle_default_case(input, i, current_part, tokens) == -1)
		return (-1);
	return (0);
}

static int	finalize_tokens(char **current_part, t_token **tokens)
{
	if (*current_part)
	{
		add_token(tokens, *current_part, TOKEN_WORD);
		if (!*tokens)
		{
			if (*current_part)
				free(*current_part);
			*current_part = NULL;
			free_token(*tokens);
			return (-1);
		}
	}
	return (0);
}

t_token	*lexer(char *input)
{
	t_token	*tokens;
	int		i;
	char	*current_part;

	tokens = NULL;
	i = 0;
	current_part = NULL;
	while (input && input[i])
	{
		if (process_input_char(input, &i, &current_part, &tokens) == -1)
			return (NULL);
	}
	if (finalize_tokens(&current_part, &tokens) == -1)
		return (NULL);
	return (tokens);
}
