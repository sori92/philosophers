/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:47:42 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/21 22:47:30 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
    Printing name and state of the philo at the current momment,
    according to the selected color. Using printf is really slow,
	so we use write but the way to custom the sentence is quite complex.
*/
void	state_printer(const char *color, t_philo *philo, const char *str)
{
	pthread_mutex_lock(&philo->manager->printer);
	printf("%s%u %d %s\x1b[0m\n", color,
		time_dif(philo->start_time), philo->name, str);
	pthread_mutex_unlock(&philo->manager->printer);
}

/*
	This function checks if the thread has to finish because of deads,
	and unlock the reserved mutexes if passed as argumments.
*/
int	check_dead(pthread_mutex_t *mut0, pthread_mutex_t *mut1, t_manager *manager)
{
	pthread_mutex_lock(&manager->dead_mutex);
	if (manager->dead != 0)
	{
		if (mut0)
			pthread_mutex_unlock(mut0);
		if (mut1)
			pthread_mutex_unlock(mut1);
		pthread_mutex_unlock(&manager->dead_mutex);
		return (0);
	}
	else
	{
		pthread_mutex_unlock(&manager->dead_mutex);
		return (1);
	}
}

/*
    Initiating some basic philo values.
    The forks are shared in a closed circle, this way:
    - The left hand always has the fork of philo name -1
    (forks start in 0 and names start in 1).
    - The right hand has the fork of philo name,
    except for the last philo, who has the fork 0.
*/
static void	init_philo_vars(t_philo *philo, t_manager manager)
{
	philo->l_hand = philo->name - 1;
	if (philo->name == manager.n_philos)
		philo->r_hand = 0;
	else
		philo->r_hand = philo->name;
	philo->lunch_count = 0;
	philo->die_time = manager.time_die;
	philo->count_reached = 0;
	pthread_mutex_init(&philo->lunch_mutex, NULL);
	pthread_mutex_lock(&philo->lunch_mutex);
	gettimeofday(&philo->start_time, NULL);
	philo->last_lunch = philo->start_time;
	pthread_mutex_unlock(&philo->lunch_mutex);
}

/*
    Main loop for the philo threads:
	First of all, we set some general vars, and after that
	we wait if the thread is even, so even and odds are async taking forks.
    It does eats and sleep, and manages to return if someone dies.
    Casting from a void is a mandatory from the 'pthread_create' function.
*/
void	*thread_loop(void *input)
{
	t_philo		*philo;
	t_manager	*manager;
	int			nerror;

	philo = (t_philo *)input;
	manager = philo->manager;
	init_philo_vars(philo, *manager);
	if (is_even(philo->name))
		usleep(1000);
	while (1)
	{
		if (!check_dead(NULL, NULL, manager))
			return (0);
		if (!eat(philo, manager))
			return (NULL);
		state_printer(BG_BLUE, philo, "is sleeping");
		if (!usleep_precise(manager->time_sleep, manager))
			return (NULL);
		state_printer(BG_CYAN, philo, "is thinking");
	}
	nerror = pthread_mutex_destroy(&philo->lunch_mutex);
	if (nerror)
		perror_destroy(nerror, "destroy mutex error in lunch_mutex");
	return (NULL);
}
