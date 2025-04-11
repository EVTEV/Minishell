/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:00:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/11 19:35:20 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

static int	write_to_tmp_file(int fd, char *line)
{
	char	*tmp;

	tmp = ft_strjoin(line, "\n");
	if (!tmp)
		return (1);
	write(fd, tmp, ft_strlen(tmp));
	free(tmp);
	return (0);
}

static int	handle_delimiter_error(char *delimiter)
{
	if (!delimiter || !*delimiter)
	{
		ft_putstr_fd(
			"minishell: syntax error near unexpected token `newline'\n",
			STDERR_FILENO);
		return (1);
	}
	return (0);
}

static int	generate_tmp_file(char *tmp_file, int *file_counter)
{
	char	*tmp;

	if (!getcwd(tmp_file, PATH_MAX))
	{
		perror("getcwd");
		return (1);
	}
	ft_strlcat(tmp_file, "/.minishell_heredoc_", PATH_MAX);
	tmp = ft_itoa((*file_counter)++);
	if (!tmp)
		return (1);
	ft_strlcat(tmp_file, tmp, PATH_MAX);
	free(tmp);
	return (0);
}

static int	process_heredoc_input(int fd, char *delimiter, size_t delimiter_len)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || (ft_strncmp(line, delimiter, delimiter_len) == 0
				&& (line[delimiter_len] == '\n'
					|| line[delimiter_len] == '\0')))
		{
			free(line);
			break ;
		}
		if (write_to_tmp_file(fd, line))
			return (handle_malloc_error(line, fd));
		free(line);
	}
	return (0);
}

int	handle_heredoc(char *delimiter, char **heredoc_file)
{
	int			fd;
	size_t		delimiter_len;
	static int	file_counter;
	char		tmp_file[PATH_MAX];

	file_counter = 0;
	if (handle_delimiter_error(delimiter))
		return (1);
	if (generate_tmp_file(tmp_file, &file_counter))
		return (1);
	if (create_tmp_file(tmp_file, heredoc_file, &fd))
		return (1);
	delimiter_len = ft_strlen(delimiter);
	if (process_heredoc_input(fd, delimiter, delimiter_len))
		return (1);
	close(fd);
	return (0);
}
