/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exp_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/03 17:51:59 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/03 17:52:06 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	expand_tokens(t_token *tokens, t_env *env)
{
	char	*expanded;

	while (tokens)
	{
		expanded = expand_token(tokens->value, env);
		free(tokens->value);
		tokens->value = expanded;
		tokens = tokens->next;
	}
}
