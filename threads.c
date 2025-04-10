/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 13:47:42 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/10 19:45:06 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	gettimeofday(&philo->start_time, NULL);
	philo->l_hand = philo->name - 1;
	if (philo->name == manager.n_philos)
		philo->r_hand = 0;
	else
		philo->r_hand = philo->name;
	philo->last_lunch = philo->start_time;
}

/*
    Printing name and state of the philo at the current momment,
    according to the selected color.
*/
void	state_printer(const char *color, t_philo *philo, const char *str)
{
	struct timeval	current_tv;
	unsigned int	current_time;

	gettimeofday(&current_tv, NULL);
	current_time = current_tv.tv_sec / 1000 + current_tv.tv_usec * 1000;
	pthread_mutex_lock(&philo->manager->printer);
	printf("%s%d %d %s\x1b[0m\n", color, current_time, philo->name, str);
	pthread_mutex_unlock(&philo->manager->printer);
}

/*
    Make 'usleep' during the given time,
    but checking if someone is dead every 10 milisecs
*/
static int	usleep_precise(unsigned int time, t_manager *manager)
{
	unsigned int	elapsed_time;

	elapsed_time = 0;
	while (elapsed_time < time)
	{
		if (time - elapsed_time < 10)
			usleep(time - elapsed_time);
		else
			usleep(10);
		elapsed_time += 10;
		if (manager->dead != 0)
			return (0);
	}
	return (1);
}

/*
    Takes the forks and eats during the 'time_eat'.
    Even takes left forks first, and odd begin with the right ones.
    If someone dies during the proccess manages to return.
*/
static int	eat(t_philo *philo, t_manager *manager)
{
	if (is_even(philo->name))
	{
		pthread_mutex_lock(&manager->forks[philo->l_hand]);
		state_printer(BG_YELLOW, philo, "has taken a fork");
		pthread_mutex_lock(&manager->forks[philo->r_hand]);
		state_printer(BG_ORANGE, philo, "has taken a fork");
	}
	else
	{
		pthread_mutex_lock(&manager->forks[philo->r_hand]);
		state_printer(BG_ORANGE, philo, "has taken a fork");
		pthread_mutex_lock(&manager->forks[philo->l_hand]);
		state_printer(BG_YELLOW, philo, "has taken a fork");
	}
	gettimeofday(&philo->last_lunch, NULL);
	state_printer(BG_BROWN, philo, "is eating");
	if (!usleep_precise(manager->time_eat, manager))
	{
		pthread_mutex_unlock(&manager->forks[philo->l_hand]);
		pthread_mutex_unlock(&manager->forks[philo->r_hand]);
		return (0);
	}
	pthread_mutex_unlock(&manager->forks[philo->l_hand]);
	pthread_mutex_unlock(&manager->forks[philo->r_hand]);
	return (1);
}

/*
    Main loop for the philo threads:
    It does eats and sleep, and manages to return if someone dies.
    Casting from a void is a mandatory from the 'pthread_create' function.
*/
void	*thread_loop(void *input)
{
	t_philo		*philo;
	t_manager	*manager;

	philo = (t_philo *)input;
	manager = philo->manager;
	init_philo_vars(philo, *manager);
	while (1)
	{
		if (!eat(philo, manager))
			return (NULL);
		state_printer(BG_BLUE, philo, "is sleeping");
		if (!usleep_precise(manager->time_sleep, manager))
			return (NULL);
		state_printer(BG_CYAN, philo, "is thinking");
	}
}
