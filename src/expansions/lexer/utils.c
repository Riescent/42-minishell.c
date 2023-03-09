/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vfries <vfries@student.42lyon.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/06 18:28:00 by tdameros          #+#    #+#             */
/*   Updated: 2023/03/09 02:00:27 by vfries           ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "expansions.h"

char	*get_end_expression(char *arg, char *delimiters)
{
	while (*arg != '\0' && ft_strchr(delimiters, *arg) == NULL)
		arg++;
	return (arg);
}

char	*get_end_dollar_parameter(char *arg)
{
	if (ft_isdigit(*arg) || *arg == '?')
		return (arg);
	while (ft_isalnum(*arg) || *arg == '_')
		arg++;
	return (arg - 1);
}

