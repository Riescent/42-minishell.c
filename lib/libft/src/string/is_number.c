/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   is_number.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tomy <tomy@student.42lyon.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/28 00:30:00 by tomy              #+#    #+#             */
/*   Updated: 2023/01/28 00:30:00 by tomy             ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "ft_char.h"

bool	ft_is_number(char *s)
{
	if (*s == '-' || *s == '+')
		s++;
	while (*s != '\0')
	{
		if (!ft_isdigit(*s))
			return (false);
		s++;
	}
	return (true);
}
