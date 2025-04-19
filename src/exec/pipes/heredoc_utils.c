/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/11 19:35:50 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/11 19:35:55 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/minishell.h"

int	handle_malloc_error(char *tmp, int fd)
{
	free(tmp);
	if (fd != -1)
		close(fd);
	return (1);
}

int	create_tmp_file(char *tmp_file, char **heredoc_file, int *fd)
{
	*fd = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0600);
	if (*fd == -1)
	{
		perror("open");
		return (1);
	}
	*heredoc_file = ft_strdup(tmp_file);
	if (!*heredoc_file)
	{
		close(*fd);
		return (1);
	}
	return (0);
}

int	handle_heredoc_in_fork(int fd, char *delimiter, size_t delimiter_len)
{
	pid_t	pid;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		return (1);
	}
	if (pid == 0)
	{
		if (process_heredoc_in_child(fd, delimiter, delimiter_len))
			exit(1);
		exit(0);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		g_exit_status = 130;
		return (1);
	}
	return (WEXITSTATUS(status));
}
