/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_manage.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 17:18:08 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/17 10:23:46 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

/*
	Current momment in time in milisecs.
*/
unsigned long	my_get_time_stamp(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

/*
    Make 'usleep' during the given time,
    but checking if someone is dead every 100 microsecs.
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
