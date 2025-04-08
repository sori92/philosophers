/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 15:34:35 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/08 13:50:16 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	ft_unsigned_atoi(const char *str)
{
	int	nb;
	int	row;
	int	i;

	nb = 0;
	row = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] <= '9' && str[i] >= '0')
		{
            if (nb > UINT_MAX / 10 || (nb == UINT_MAX / 10 && (str[i] - '0') > UINT_MAX % 10))
                return (0, error_args("unsigned int overflow"));
			nb = (str[i] - '0') + (nb * 10);
			i++;
			row = 1;
		}
		else if ((row == 0) && ((9 <= str[i] && str[i] <= 13) || str[i] == ' '
				|| str[i] == '+'))
			i++;
		else
			return (0, error_args("not a valid number"));
	}
	return (nb);
}
