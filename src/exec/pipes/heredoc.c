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
#include <signal.h>

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

/* Static variable to track heredoc interruption */
static sig_atomic_t heredoc_interrupted = 0;

/* Gestionnaire de signal pour interrompre le heredoc */
static void	signal_handler_heredoc(int signum)
{
	if (signum == SIGINT)
	{
		heredoc_interrupted = 1;
		write(1, "\n", 1); // Affiche une nouvelle ligne immédiatement
	}
}

/* Lit l'entrée du heredoc jusqu'au délimiteur */
static int	read_heredoc_input(int pipe_fd[2], char *delimiter)
{
	char	*line;
	size_t	delimiter_len;

	delimiter_len = ft_strlen(delimiter);
	while (!heredoc_interrupted)
	{
		ft_printf("> ");
		line = get_next_line(0);
		if (!line)
		{
			if (errno != 0)
			{
				perror("get_next_line");
				close(pipe_fd[1]);
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
			close(pipe_fd[1]);
			return (-1);
		}
		free(line);
	}
	close(pipe_fd[1]); // Ferme le côté écriture du pipe après la lecture
	return (heredoc_interrupted ? -2 : 0); // Retourne -2 si interrompu
}

/* Gère le processus de heredoc complet */
int	handle_heredoc(char *delimiter, int *fd_in)
{
	int		pipe_fd[2];
	int		result;
	void	(*prev_sigint_handler)(int);

	if (!delimiter || !*delimiter)
	{
		ft_putstr_fd("minishell: syntax error near unexpected token `newline'\n", STDERR_FILENO);
		return (1);
	}
	if (setup_heredoc_pipe(pipe_fd) != 0)
		return (1);

	// Configure le gestionnaire de signal pour le heredoc
	heredoc_interrupted = 0;
	prev_sigint_handler = signal(SIGINT, signal_handler_heredoc);

	result = read_heredoc_input(pipe_fd, delimiter);

	// Restaure le gestionnaire de signal précédent
	signal(SIGINT, prev_sigint_handler);

	close(pipe_fd[1]);
	if (result == -2) // Si le heredoc a été interrompu
	{
		close(pipe_fd[0]);
		return (1);
	}
	if (result < 0)
	{
		close(pipe_fd[0]);
		return (1);
	}
	*fd_in = pipe_fd[0];
	return (0);
}
