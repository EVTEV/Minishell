/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 17:00:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 11:05:47 by lowatell         ###   ########.fr       */
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

/* Lit l'entrée du heredoc jusqu'au délimiteur */
static int	read_heredoc_input(int pipe_fd[2], char *delimiter)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	while (1)
	{
		ft_printf("> ");
		line = get_next_line(0);
		if (!line)
		{
			if (errno != 0)
			{
				perror("get_next_line");
				close(pipe_fd[1]); // Ferme le pipe en cas d'erreur
				return (-1);
			}
			break;
		}
		if (ft_strncmp(line, delimiter, delimiter_len) == 0
			&& (line[delimiter_len] == '\n' || line[delimiter_len] == '\0'))
		{
			free(line);
			break; // Arrête la lecture lorsque le délimiteur est atteint
		}
		if (write(pipe_fd[1], line, ft_strlen(line)) < 0)
		{
			free(line);
			perror("write");
			close(pipe_fd[1]); // Ferme le pipe en cas d'erreur
			return (-1);
		}
		free(line);
	}
	close(pipe_fd[1]); // Ferme le côté écriture du pipe après la lecture
	return (0);
}

/* Gère le processus de heredoc complet */
int	handle_heredoc(char *delimiter, int *fd_in)
{
	int		pipe_fd[2];
	int		result;

	if (!delimiter || !*delimiter) // Vérifie si le délimiteur est vide
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (1);
	}
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
	if (*fd_in < 0) // Vérifie si le fichier est correctement créé
	{
		ft_putstr_fd("minishell: error creating heredoc file\n", STDERR_FILENO);
		return (1);
	}
	return (0);
}
