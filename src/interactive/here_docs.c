/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_docs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/20 09:17:54 by vfries            #+#    #+#             */
/*   Updated: 2023/03/09 01:47:12 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "expansions.h"
#include <stdio.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <errno.h>

#include "minishell_signal.h"
#include "error.h"
#include "interactive.h"

#define HERE_DOCS_PROMPT "> "

static int	add_here_doc(t_minishell *minishell, char *limiter);
static int	get_input(t_list **input_lst, char *limiter,
				t_minishell *minishell);
static int	add_input(t_list **input_lst, int *pipe_fd, pid_t pid);
static int	get_forked_input(int *pipe_fd, char *limiter);

int	get_here_docs(t_list *tokens, t_minishell *minishell)
{
	t_token	*token;
	t_token	*next_token;
	int		return_code;
	char	*limiter;

	return_code = 0;
	while (tokens->next != NULL)
	{
		token = tokens->content;
		next_token = tokens->next->content;
		if (token->operator == HERE_DOC)
		{
			limiter = strdup_without_quotes(next_token->name);
			if (limiter == NULL)
				return (-1);
			return_code = add_here_doc(minishell, limiter);
			free(limiter);
		}
		if (return_code != 0)
			return (return_code);
		tokens = tokens->next;
	}
	return (0);
}

static int	add_here_doc(t_minishell *minishell, char *limiter)
{
	t_list	*input;
	t_list	*new_node;
	int		return_code;

	input = NULL;
	return_code = get_input(&input, limiter, minishell);
	if (return_code != 0)
	{
		ft_lstclear(&input, &free);
		return (return_code);
	}
	ft_lst_reverse(&input);
	new_node = ft_lstnew(input);
	if (new_node == NULL)
	{
		ft_lstclear(&input, &free);
		return (1);
	}
	ft_lstadd_front(&minishell->here_docs, new_node);
	return (0);
}

static int	get_input(t_list **input_lst, char *limiter, t_minishell *minishell)
{
	int		pipe_fd[2];
	pid_t	pid;
	int		exit_code;

	if (pipe(pipe_fd) == -1)
		return (1);
	pid = fork();
	if (pid == -1)
		return (close_pipe(pipe_fd), 1);
	else if (pid == 0)
	{
		exit_code = get_forked_input(pipe_fd, limiter);
		free_minishell(minishell);
		exit(exit_code);
	}
	return (add_input(input_lst, pipe_fd, pid));
}

static int	get_forked_input(int *pipe_fd, char *limiter)
{
	char	*input;

	errno = 0;
	init_interactive_fork_signal_handling();
	if (errno != 0)
		return (close_pipe(pipe_fd), 1);
	input = readline(HERE_DOCS_PROMPT);
	while (input != NULL && ft_strcmp(input, limiter) != 0)
	{
		ft_putstr_fd(input, pipe_fd[1]);
		ft_putstr_fd("\n", pipe_fd[1]);
		if (errno != 0)
			return (free(input), close_pipe(pipe_fd), 1);
		free(input);
		input = readline(HERE_DOCS_PROMPT);
	}
	if (input == NULL)
		print_error(NULL, "warning", "here-document delimited by end-of-file");
	free(input);
	close_pipe(pipe_fd);
	if (errno != 0)
		return (1);
	return (0);
}

static int	add_input(t_list **input_lst, int *pipe_fd, pid_t pid)
{
	char	*input;
	t_list	*new_node;
	int		exit_status;

	if (close(pipe_fd[1]) < 0)
	{
		close(pipe_fd[0]);
		return (1);
	}
	input = get_next_line(pipe_fd[0]);
	while (input != NULL)
	{
		new_node = ft_lstnew(input);
		if (new_node == NULL)
			return (1);
		ft_lstadd_front(input_lst, new_node);
		input = get_next_line(pipe_fd[0]);
	}
	close(pipe_fd[0]);
	if (waitpid(pid, &exit_status, 0) < 0)
		return (1);
	exit_status = WEXITSTATUS(exit_status);
	if (exit_status == 130 || exit_status == 1)
		return (close_pipe(pipe_fd), exit_status);
	return (0);
}
