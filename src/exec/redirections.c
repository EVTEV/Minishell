/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirections.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flash19 <flash19@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/27 14:40:42 by flash19           #+#    #+#             */
/*   Updated: 2023/03/27 14:40:42 by flash19          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

int	setup_redirections(t_redir *redirections)
{
	t_redir	*current;

	current = redirections;
	while (current)
	{
		if (current->type == 0) // Input redirection (<)
		{
			if (handle_input_redirection(current->file) != 0)
				return (1);
		}
		else if (current->type == 1) // Output redirection (>)
		{
			if (handle_output_redirection(current->file, 0) != 0)
				return (1);
		}
		else if (current->type == 2) // Append redirection (>>)
		{
			if (handle_output_redirection(current->file, 1) != 0)
				return (1);
		}
		else if (current->type == 3) // Heredoc (<<)
		{
			if (handle_heredoc_redirection(current->file) != 0)
				return (1);
		}
		current = current->next;
	}
	return (0);
}