/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:52:19 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/03 17:52:20 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '"');
}

void	handle_quote_state(char c, char *quote)
{
	if (!*quote && is_quote(c))
		*quote = c;
	else if (*quote && c == *quote)
		*quote = 0;
}

char	*allocate_result_buffer(char *value)
{
	char	*result;

	result = (char *)malloc(ft_strlen(value) + 1);
	if (!result)
		return (NULL);
	return (result);
}

void	combine_token_values(t_ast_node *current)
{
	char	*left_value;
	char	*right_value;
	char	*combined_value;

	left_value = remove_quotes(current->token->value);
	right_value = remove_quotes(current->right->token->value);
	combined_value = ft_strjoin(left_value, right_value);
	free(left_value);
	free(right_value);
	free(current->token->value);
	current->token->value = combined_value;
}
