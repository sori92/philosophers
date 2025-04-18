/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/17 13:37:26 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/18 13:23:04 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Sets the 'dead' bool to 1 and unlock the mutexes used.
*/
static void	set_dead(t_philo philo, t_manager *manager)
{
	pthread_mutex_lock(&manager->dead_mutex);
	manager->dead = 1;
	pthread_mutex_unlock(&manager->dead_mutex);
	pthread_mutex_unlock(&manager->satisfied);
	pthread_mutex_unlock(&manager->forks[philo.l_hand]);
	pthread_mutex_unlock(&manager->forks[philo.r_hand]);
}

/*
    Eats during the 'time_eat'.
    If someone dies during the proccess manages to return with 0.
*/
static int	eat_aux(t_philo *philo, t_manager *manager)
{
	pthread_mutex_lock(&philo->lunch_mutex);
	gettimeofday(&philo->last_lunch, NULL);
	pthread_mutex_unlock(&philo->lunch_mutex);
	state_printer(BG_BROWN, philo, "is eating");
	if (!usleep_precise(manager->time_eat, manager))
	{
		pthread_mutex_unlock(&manager->forks[philo->l_hand]);
		return (pthread_mutex_unlock(&manager->forks[philo->r_hand]), 0);
	}
	philo->lunch_count++;
	if ((philo->lunch_count == manager->goal_lunchs)
		&& philo->count_reached == 0)
	{
		pthread_mutex_lock(&manager->satisfied);
		manager->philo_satisfied++;
		if (manager->philo_satisfied >= manager->n_philos)
			return (set_dead(*philo, manager), 0);
		pthread_mutex_unlock(&manager->satisfied);
		philo->count_reached = 1;
	}
	pthread_mutex_unlock(&manager->forks[philo->l_hand]);
	pthread_mutex_unlock(&manager->forks[philo->r_hand]);
	return (1);
}

/*
    Takes the forks: Even takes left forks first,
	and odd begin with the right ones.
    If someone dies during the proccess manages to return with 0.
*/
int	eat(t_philo *philo, t_manager *manager)
{
	if (is_even(philo->name))
	{
		pthread_mutex_lock(&manager->forks[philo->l_hand]);
		if (!check_dead(&manager->forks[philo->l_hand], NULL, manager))
			return (0);
		state_printer(BG_YELLOW, philo, "has taken a fork");
		pthread_mutex_lock(&manager->forks[philo->r_hand]);
		if (!check_dead(&manager->forks[philo->l_hand],
				&manager->forks[philo->r_hand], manager))
			return (0);
		state_printer(BG_ORANGE, philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&manager->forks[philo->r_hand]);
		if (!check_dead(NULL, &manager->forks[philo->r_hand], manager))
			return (0);
		state_printer(BG_ORANGE, philo, "has taken a fork");
		pthread_mutex_lock(&manager->forks[philo->l_hand]);
		if (!check_dead(&manager->forks[philo->l_hand],
				&manager->forks[philo->r_hand], manager))
			return (0);
		state_printer(BG_YELLOW, philo, "has taken a fork");
	}
	return (eat_aux(philo, manager));
}
