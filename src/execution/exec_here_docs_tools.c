/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_here_docs_tools.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/19 21:40:50 by vfries            #+#    #+#             */
/*   Updated: 2023/02/19 21:48:05 by vfries           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "lexer.h"
#include "error.h"
#include "execution.h"

static void	free_one_here_doc(t_list **here_docs);

void	skip_tokens_here_docs(t_list *tokens, t_list **here_docs)
{
	if (here_docs == NULL || *here_docs == NULL)
		return ;
	while (tokens != NULL)
	{
		skip_token_here_docs(tokens->content, here_docs);
		tokens = tokens->next;
	}
}

void	skip_token_here_docs(t_token *token, t_list **here_docs)
{
	t_list	*files;

	if (token == NULL || here_docs == NULL || *here_docs == NULL)
		return ;
	skip_tokens_here_docs(token->subshell, here_docs);
	files = token->files;
	while (files != NULL)
	{
		token = files->content;
		if (token->operator == HERE_DOC)
			free_one_here_doc(here_docs);
		files = files->next;
	}
}

static void	free_one_here_doc(t_list **here_docs)
{
	t_list	*here_doc;
	t_list	*tmp;

	if (here_docs == NULL || *here_docs == NULL)
		return ;
	here_doc = (*here_docs)->content;
	ft_lstclear(&here_doc, &free);
	tmp = *here_docs;
	*here_docs = (*here_docs)->next;
	free(tmp);
}

int	read_here_doc(t_list **here_docs)
{
	t_list	*here_doc;
	int		pipe_fd[2];

	if (here_docs == NULL || *here_docs == NULL)
		return (-1);
	if (pipe(pipe_fd) == -1)
	{
		free_one_here_doc(here_docs);
		return (-1);
	}
	here_doc = (*here_docs)->content;
	while (here_doc != NULL)
	{
		ft_putstr_fd(here_doc->content, pipe_fd[1]);
		here_doc = here_doc->next;
	}
	free_one_here_doc(here_docs);
	if (close(pipe_fd[1]) < 0)
	{
		close(pipe_fd[0]);
		return (-1);
	}
	return (pipe_fd[0]);
}
