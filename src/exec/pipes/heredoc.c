/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:00:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 17:00:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

/* Configure le pipe pour le heredoc */
static int	setup_heredoc_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) < 0)
	{
		perror("pipe");
		return (1);
	}
	return (0);
}

/* Traite une ligne d'entrée du heredoc */
static int	process_line(int pipe_fd[2], char *line, char *delimiter, 
						size_t delimiter_len)
{
	if (ft_strncmp(line, delimiter, delimiter_len) == 0 && 
		(line[delimiter_len] == '\n' || line[delimiter_len] == '\0'))
	{
		free(line);
		return (1);
	}
	if (write(pipe_fd[1], line, ft_strlen(line)) < 0)
	{
		free(line);
		perror("write");
		return (-1);
	}
	free(line);
	return (0);
}

/* Lit l'entrée du heredoc jusqu'au délimiteur */
static int	read_heredoc_input(int pipe_fd[2], char *delimiter)
{
	char	*line;
	size_t	delimiter_len;
	int		result;

	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		ft_printf("heredoc> ");
		line = get_next_line(0);
		if (!line)
		{
			if (errno != 0)
			{
				perror("get_next_line");
				return (-1);
			}
			break;
		}
		result = process_line(pipe_fd, line, delimiter, delimiter_len);
		if (result != 0)
			return (result);
	}
	return (0);
}

/* Gère le processus de heredoc complet */
int	handle_heredoc(char *delimiter, int *fd_in)
{
	int		pipe_fd[2];
	int		result;

	if (setup_heredoc_pipe(pipe_fd) != 0)
		return (1);
	
	result = read_heredoc_input(pipe_fd, delimiter);
	close(pipe_fd[1]);
	
	if (result < 0)
	{
		close(pipe_fd[0]);
		return (1);
	}
	
	*fd_in = pipe_fd[0];
	return (0);
} 