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
	int	std_fds[2];
	int	result = 0;


	std_fds[0] = dup(STDIN_FILENO);
	std_fds[1] = dup(STDOUT_FILENO);
	if (std_fds[0] == -1 || std_fds[1] == -1)
	{
		perror("dup");
		return (-1);
	}
	while (redirections)
	{
		if (redirections->type == TOKEN_REDIR_IN)
			result = handle_input_redirection(redirections->file);
		else if (redirections->type == TOKEN_REDIR_OUT)
			result = handle_output_redirection(redirections->file, 0);
		else if (redirections->type == TOKEN_REDIR_APPEND)
			result = handle_output_redirection(redirections->file, 1);
		if (result == -1)
			break;
		redirections = redirections->next;
	}
	if (dup2(std_fds[0], STDIN_FILENO) == -1 || dup2(std_fds[1], STDOUT_FILENO) == -1)
	{
		perror("dup2");
		result = -1;
	}
	close(std_fds[0]);
	close(std_fds[1]);
	return (result);
}
