/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expander_str.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 21:42:53 by lowatell          #+#    #+#             */
/*   Updated: 2025/03/26 22:31:53 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_quote_state(char c, int *sq, int *dq)
{
	if (c == '"' && !*sq)
		*dq = !*dq;
	else if (c == '\'' && !*dq)
		*sq = !*sq;
}

static char	*expand_loop(const char *input, t_env *env, int status)
{
	int		i;
	int		sq;
	int		dq;
	char	*res;
	char	*part;

	i = 0;
	sq = 0;
	dq = 0;
	res = ft_strdup("");
	while (input[i])
	{
		update_quote_state(input[i], &sq, &dq);
		if (input[i] == '$' && !sq && is_var_start(input[i + 1]))
		{
			part = get_var_name(input, &i);
			part = get_var_value(part, env, status);
			res = append_and_free(res, part);
			continue ;
		}
		part = ft_substr(input, i, 1);
		res = append_and_free(res, part);
		i++;
	}
	return (res);
}

char	*expand_string(const char *input, t_env *env, int last_status)
{
	return (expand_loop(input, env, last_status));
}
