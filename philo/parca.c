/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parca.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:29:37 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/21 22:48:03 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Sets the 'dead' bool to 1 and prints the dead.
*/
static void	set_dead(t_manager *manager, t_philo **philo, unsigned int i)
{
	pthread_mutex_lock(&manager->dead_mutex);
	manager->dead = 1;
	pthread_mutex_unlock(&manager->dead_mutex);
	state_printer(BG_RED, philo[i], "died");
	return ;
}

/*
	Loop that is always checking if someone has to die from starvation.
	The moment that´s considered for starting the starvation
	is the moment of starting to eat (yes, I know it has no much sense,
	but that´s how it´s supposed to work).
*/
void	parca_loop(t_manager *manager, t_philo **philo)
{
	unsigned int	dif;
	unsigned int	i;

	usleep(10000);
	while (1)
	{
		pthread_mutex_lock(&manager->dead_mutex);
		if (manager->dead == 0)
		{
			pthread_mutex_unlock(&manager->dead_mutex);
			i = 0;
			while (i < manager->n_philos)
			{
				pthread_mutex_lock(&philo[i]->lunch_mutex);
				dif = time_dif(philo[i]->last_lunch);
				pthread_mutex_unlock(&philo[i]->lunch_mutex);
				if (dif > philo[i]->die_time)
					return (set_dead(manager, philo, i));
				i++;
			}
		}
		else
			return ((void)pthread_mutex_unlock(&manager->dead_mutex));
	}
}
