/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:40:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 18:34:35 by lowatell         ###   ########.fr       */
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
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

/* Configure les redirections pour une commande */
int	setup_redirections(t_redir *redirections)
{
	while (redirections)
	{
		if (redirections->type == TOKEN_REDIR_IN)
		{
			if (handle_input_redirection(redirections->file) == -1)
				return (-1);
		}
		else if (redirections->type == TOKEN_REDIR_OUT)
		{
			if (handle_output_redirection(redirections->file, 0) == -1)
				return (-1);
		}
		else if (redirections->type == TOKEN_REDIR_APPEND)
		{
			if (handle_output_redirection(redirections->file, 1) == -1)
				return (-1);
		}
		redirections = redirections->next;
	}
	return (0);
}
