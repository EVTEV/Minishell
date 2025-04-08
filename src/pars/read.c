/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lowatell <lowatell@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/08 14:46:42 by lowatell          #+#    #+#             */
/*   Updated: 2025/04/08 08:09:38 by lowatell         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"
#include <signal.h>
#include <termios.h> // Ajout pour gérer les attributs du terminal

int	g_exit_status = 0; // Définition de la variable globale

void	reset_terminal_line(void)
{
	struct termios	term;

	tcgetattr(STDIN_FILENO, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

void	signal_handler_main(int signum)
{
	struct termios	term;

	if (signum == SIGINT)
	{
		rl_replace_line("", 0); // Efface la ligne courante
		write(1, "\n", 1); // Affiche une nouvelle ligne
		rl_on_new_line();   // Indique à readline qu'une nouvelle ligne commence
		rl_redisplay();     // Réaffiche le prompt
		g_exit_status = 1;  // Met à jour le statut global
	}
	tcgetattr(1, &term);
	term.c_lflag &= ~ECHOCTL;
	tcsetattr(1, TCSANOW, &term);
}

void	ft_exec_sig_handler(int sig)
{
	if (sig == SIGINT)
	{
		write(1, "\n", 1); // Ajoute une nouvelle ligne pour éviter les doublons
		g_exit_status = 130;
	}
	else if (sig == SIGQUIT)
	{
		write(2, "Quit: 3\n", 8);
		g_exit_status = 131;
	}
}

void	load_history(void)
{
	int		fd;
	char	*line;

	fd = open("./.shell_history", O_CREAT | O_RDONLY, 0644);
	if (fd == -1)
		return ;
	line = get_next_line(fd);
	if (!line)
		close(fd);
	while (line)
	{
		add_history(ft_substr(line, 0, ft_strlen(line) - 1));
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
}

void	save_history(char *input)
{
	int		fd;
	char	*tmp;

	if (!input)
		return ;
	fd = open("./.shell_history", O_CREAT | O_WRONLY | O_APPEND, 0644);
	if (fd == -1)
		return ;
	tmp = ft_strjoin(input, "\n");
	if (tmp)
	{
		ft_putstr_fd(tmp, fd);
		free(tmp);
		tmp = NULL;
	}
	close(fd);
}

char	*read_input(t_data *data)
{
	char	*input;

	reset_terminal_line();
	input = readline("minishell> ");
	if (!input)
	{
		write(1, "exit\n", 5);
		exit_clean(data);
	}
	if (*input)
	{
		add_history(input);
		save_history(input);
	}
	return (input);
}
