/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:00:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/11 10:26:12 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

/* Gère le processus de heredoc complet */
int	handle_heredoc(char *delimiter, char **heredoc_file)
{
	char	*line;
	int		fd;
	size_t	delimiter_len;
	static int	file_counter = 0; // Static counter for unique file names
	char	tmp_file[PATH_MAX];

	if (!delimiter || !*delimiter)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (1);
	}

	// Get the current working directory
	if (!getcwd(tmp_file, PATH_MAX))
	{
		perror("getcwd");
		return (1);
	}

	// Append the temporary file name to the directory path
	ft_strlcat(tmp_file, "/.minishell_heredoc_", PATH_MAX);
	ft_strlcat(tmp_file, ft_itoa(file_counter++), PATH_MAX); // Increment counter for uniqueness

	// Open the temporary file
	fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}

	*heredoc_file = ft_strdup(tmp_file);
	if (!*heredoc_file)
	{
		close(fd);
		return (1);
	}

	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		ft_printf("> ");
		line = get_next_line(0);
		if (!line || (ft_strncmp(line, delimiter, delimiter_len) == 0
			&& (line[delimiter_len] == '\n' || line[delimiter_len] == '\0')))
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		free(line);
	}
	close(fd);
	return (0);
}
