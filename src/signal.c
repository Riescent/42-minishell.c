/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/04 17:38:45 by vfries            #+#    #+#             */
/*   Updated: 2023/02/15 00:06:15 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include "built_in.h"
#include "error.h"
#include "exit_code.h"
#include "get_cursor_x_pos.h"
#include "sys/wait.h"
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <sys/wait.h>

static void	signal_handler(int sig);
static void	interactive_signal_handler(int sig);
static void	signal_handler_with_active_fork(int sig);

void	init_interactive_signal_handling(void)
{
	struct sigaction	action;

	action.sa_handler = &interactive_signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);
}

static void	interactive_signal_handler(int sig)
{
	if (sig != SIGINT)
		return ;
	exit(130);
}

int	signal_init_handling(void)
{
	struct sigaction	action;

	action.sa_handler = &signal_handler;
	sigemptyset(&action.sa_mask);
	action.sa_flags = SA_RESTART;
	if (sigaction(SIGINT, &action, NULL) < 0)
	{
		print_error(NULL, "sigaction() failed", get_error());
		return (-1);
	}
	if (sigaction(SIGQUIT, &action, NULL) < 0)
	{
		print_error(NULL, "sigaction() failed", get_error());
		return (-1);
	}
	return (0);
}

static void	signal_handler(int sig)
{
	int			wait_exit_code;
	const pid_t	pid = waitpid(-1, &wait_exit_code, WUNTRACED);

	if (pid == -1 && WIFEXITED(wait_exit_code))
		return ;
	if (pid != -1)
		return (signal_handler_with_active_fork(sig));
	if (sig == SIGQUIT)
		return ;
	exit_code(130);
	rl_replace_line("", 0);
	ft_putstr("\n");
	rl_on_new_line();
	rl_redisplay();
}

static void	signal_handler_with_active_fork(int sig)
{
	if (sig == SIGINT)
	{
		if (get_cursor_x_pos() > 1)
			ft_putchar_fd('\n', STDOUT_FILENO);
		exit_code(130);
	}
	else if (sig == SIGQUIT)
	{
		if (get_cursor_x_pos() > 1)
			ft_printf("Quit: 3\n");
		exit_code(131);
	}
}
