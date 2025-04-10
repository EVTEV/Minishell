/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:40:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/10 23:01:58 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* Gère la redirection d'entrée (<) */
static int	handle_input_redirection(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Gère la redirection de sortie (>) */
static int	handle_output_redirection(char *filename, int append)
{
	int	fd;

	if (append)
		fd = open(filename, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else
		fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror(filename);
		return (-1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1) // Redirige STDOUT vers le fichier
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Crée les fichiers pour les redirections de sortie (TOKEN_REDIR_OUT et TOKEN_REDIR_APPEND) */
static int	create_redirection_files(t_redir *redirections)
{
	int	fd;

	while (redirections)
	{
		if (redirections->type == TOKEN_REDIR_OUT)
		{
			fd = open(redirections->file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
			if (fd == -1)
			{
				perror(redirections->file);
				return (-1);
			}
			close(fd);
		}
		else if (redirections->type == TOKEN_REDIR_APPEND)
		{
			fd = open(redirections->file, O_CREAT | O_WRONLY | O_APPEND, 0644);
			if (fd == -1)
			{
				perror(redirections->file);
				return (-1);
			}
			close(fd);
		}
		redirections = redirections->next;
	}
	return (0);
}

/* Configure les redirections pour une commande */
int	setup_redirections(t_redir *redirections)
{
	int	result = 0;

	// Crée les fichiers de redirection avant de configurer les redirections
	if (create_redirection_files(redirections) != 0)
		return (1); // Return 1 to indicate an error but allow pipeline to continue

	while (redirections)
	{
		if (redirections->type == TOKEN_REDIR_IN)
			result = handle_input_redirection(redirections->file);
		else if (redirections->type == TOKEN_REDIR_OUT)
			result = handle_output_redirection(redirections->file, 0);
		else if (redirections->type == TOKEN_REDIR_APPEND)
			result = handle_output_redirection(redirections->file, 1);
		else if (redirections->type == TOKEN_REDIR_HEREDOC)
		{
			char *heredoc_file;
			if (handle_heredoc(redirections->file, &heredoc_file) != 0)
				return (1); // Return 1 to indicate an error but allow pipeline to continue
			int fd_in = open(heredoc_file, O_RDONLY);
			free(heredoc_file); // Free the temporary file path after opening
			if (fd_in == -1)
			{
				perror("open");
				return (1); // Return 1 to indicate an error but allow pipeline to continue
			}
			if (dup2(fd_in, STDIN_FILENO) == -1)
			{
				perror("dup2");
				close(fd_in);
				return (1); // Return 1 to indicate an error but allow pipeline to continue
			}
			close(fd_in);
		}
		if (result == -1)
			return (1); // Return 1 to indicate an error but allow pipeline to continue
		redirections = redirections->next;
	}
	return (0);
}
