/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 19:45:18 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/16 14:30:33 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

/*
	Check the difference between a given moment
	and the current moment, returning it in milisecs.
*/
unsigned int	time_dif(struct timeval start_tv)
{
	struct timeval	current_tv;
	unsigned int	start_time;
	unsigned int	current_time;

	gettimeofday(&current_tv, NULL);
	start_time = start_tv.tv_sec * 1000 + start_tv.tv_usec / 1000;
	current_time = current_tv.tv_sec * 1000 + current_tv.tv_usec / 1000;
	return (current_time - start_time);
}

unsigned long	my_get_time_stamp()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
    Make 'usleep' during the given time,
    but checking if someone is dead every 100 microsecs
*/
int	usleep_precise(unsigned long time, t_manager *manager)
{
	unsigned long	start;
	unsigned long	current;

	start = my_get_time_stamp();
	current = 0;
	while (current < time)
	{
		if (!check_dead(NULL, NULL, manager))
			return (0);
		usleep(100);
		current = my_get_time_stamp() - start;
	}
	return (1);
}
