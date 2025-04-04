/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:46:42 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/04 11:02:53 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	load_history(void)
{
	int		fd;
	char	*line;

	fd = open(".shell_history", O_CREAT | O_RDONLY, 0644);
	if (fd == -1)
		return ;
	line = get_next_line(fd);
	if (!line)
		close(fd);
	while (line)
	{
		add_history(ft_substr(line, 0, ft_strlen(line) - 1));
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	save_history(char *input)
{
	int		fd;
	char	*tmp;

	if (!input)
		return ;
	fd = open(".shell_history", O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
		return ;
	tmp = ft_strjoin(input, "\n");
	if (tmp)
	{
		ft_putstr_fd(tmp, fd);
		free(tmp);
		tmp = NULL;
	}
	close(fd);
}

char	*read_input(void)
{
	char	*input;

	input = readline("minishell> ");
	if (!input)
		return (NULL);
	if (*input)
	{
		add_history(input);
		save_history(input);
	}
	return (input);
}
