/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:40:42 by flash19           #+#    #+#             */
/*   Updated: 2025/04/04 10:21:44 by lowatell         ###   ########.fr       */
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
		ft_printf("minishell: %s: %s\n", filename, strerror(errno));
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/* Gère la redirection de sortie (>) et l'ajout (>>) */
static int	handle_output_redirection(char *filename, int append)
{
	int	fd;
	int	flags;

	flags = O_WRONLY | O_CREAT;
	if (append)
		flags |= O_APPEND;
	else
		flags |= O_TRUNC;
	fd = open(filename, flags, 0644);
	if (fd == -1)
	{
		ft_printf("minishell: %s: %s\n", filename, strerror(errno));
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

/* Gère la redirection heredoc (<<) */
static int	handle_heredoc_redirection(char *delimiter)
{
	int	fd_in;
	int	result;

	result = handle_heredoc(delimiter, &fd_in);
	if (result != 0)
		return (1);
	if (dup2(fd_in, STDIN_FILENO) == -1)
	{
		close(fd_in);
		return (1);
	}
	close(fd_in);
	return (0);
}

/* Gère une redirection spécifique en fonction de son type */
static int	handle_redirection(t_redir *redir)
{
	if (redir->type == 0)
		return (handle_input_redirection(redir->file));
	else if (redir->type == 1)
		return (handle_output_redirection(redir->file, 0));
	else if (redir->type == 2)
		return (handle_output_redirection(redir->file, 1));
	else if (redir->type == 3)
		return (handle_heredoc_redirection(redir->file));
	return (1);
}

/* Configure toutes les redirections pour une commande */
int	setup_redirections(t_redir *redirections)
{
	t_redir	*current;

	current = redirections;
	while (current)
	{
		if (handle_redirection(current) != 0)
			return (1);
		current = current->next;
	}
	return (0);
}
