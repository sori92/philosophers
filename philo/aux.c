/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:45:18 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/21 19:35:56 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned int	ft_unsigned_itoa(unsigned int n, char *str)
{
	unsigned int	len;
	unsigned int	temp;

	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (1);
	}
	len = 0;
	temp = n;
	while (temp)
	{
		temp = temp / 10;
		len++;
	}
	str[len] = '0';
	while (n)
	{
		str[--len] = (n % 10) + '0';
		n = n / 10;
	}
	return (len + 1);
}

unsigned int	ft_unsigned_atoi(const char *str)
{
	unsigned int	nb;
	unsigned int	row;
	unsigned int	i;

	nb = 0;
	row = 0;
	i = 0;
	while (str && str[i])
	{
		if (str[i] <= '9' && str[i] >= '0')
		{
			if (nb > UINT_MAX / 10 || (nb == UINT_MAX / 10
					&& ((unsigned int)str[i] - '0') > UINT_MAX % 10))
				return (perror_args("unsigned int overflow"), 0);
			nb = (str[i] - '0') + (nb * 10);
			i++;
			row = 1;
		}
		else if ((row == 0) && ((9 <= str[i] && str[i] <= 13) || str[i] == ' '
				|| str[i] == '+'))
			i++;
		else
			return (perror_args("not a valid number"), 0);
	}
	return (nb);
}

int	is_even(unsigned int name)
{
	if (name % 2 == 0)
		return (1);
	return (0);
}
