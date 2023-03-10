/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/27 17:01:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/03/09 01:27:48 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "lexer.h"
#include "error.h"

void	print_error(const char *command, const char *context, const char *error)
{
	ft_putstr_fd("minishell: ", STDERR_FILENO);
	if (command != NULL)
	{
		ft_putstr_fd(command, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (context != NULL)
	{
		ft_putstr_fd(context, STDERR_FILENO);
		ft_putstr_fd(": ", STDERR_FILENO);
	}
	if (error != NULL)
		ft_putstr_fd(error, STDERR_FILENO);
	ft_putstr_fd("\n", STDERR_FILENO);
}

char	*get_name(t_list *token)
{
	return (((t_token *)token->content)->args[0]);
}

char	*get_error(void)
{
	return (strerror(errno));
}
