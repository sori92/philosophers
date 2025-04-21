/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:29 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/21 23:51:45 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Destryoing all the mutex, and cleaning allocations.
*/
static int	clean_mutexes(t_manager *manager, t_philo **philo,
				int nerror, unsigned int i)
{
	while (i < manager->n_philos)
	{
		if (manager->forks)
		{
			nerror = pthread_mutex_destroy(&manager->forks[i]);
			if (nerror)
				return (perror_destroy(nerror, "destroy mutex error in forks"));
		}
		i++;
	}
	free(manager->forks);
	nerror = pthread_mutex_destroy(&manager->printer);
	if (nerror)
		return (perror_destroy(nerror, "destroy mutex error in printer"));
	i = 0;
	while (i < manager->n_philos)
	{
		free(philo[i]);
		i++;
	}
	free(philo);
	nerror = pthread_mutex_destroy(&manager->dead_mutex);
	if (nerror)
		return (perror_destroy(nerror, "destroy mutex error in dead"));
	return (0);
}

/*
	Destryoing all the threads, then calling clean_mutexes.
*/
static int	clean_program(t_manager *manager, t_philo **philo, int nerror)
{
	unsigned int	i;

	i = 0;
	while (i < manager->n_philos)
	{
		if (manager->threads[i])
		{
			nerror = pthread_join(manager->threads[i], NULL);
			if (nerror)
				return (perror_destroy(nerror, "join error in threads"));
		}
		i++;
	}
	free(manager->threads);
	nerror = clean_mutexes(manager, philo, nerror, 0);
	if (nerror)
		return (nerror);
	return (0);
}

/*
	Create the 't_philo' struct for each philosoopher.
*/
static int	create_philos(t_philo **philo, t_manager *manager)
{
	unsigned int	i;
	int				nerror;

	i = 0;
	while (i < manager->n_philos)
	{
		nerror = pthread_mutex_init(&manager->forks[i], NULL);
		if (nerror)
			return (perror_alloc_create(nerror, "init mutex error in forks"));
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			return (perror_alloc_create(0, "allocation issue in philo[i]"));
		philo[i]->name = i + 1;
		philo[i]->manager = manager;
		i++;
	}
	return (0);
}

/*
    Initiating some basic main values:
    - Allocation of space for 'philo**', 'threads*' and 'forks*'.
	- Creation of a 'mutex' for the prints, and another for 'dead' bool.
	- Setting the 'dead' to 0.
*/
static int	init_main_vars(t_manager *manager, t_philo ***philo, int nerror)
{
	*philo = malloc(sizeof(t_philo *) * manager->n_philos);
	if (!(*philo))
		return (perror_alloc_create(0, "allocation issue in philo"));
	manager->threads = malloc(sizeof(pthread_t) * manager->n_philos);
	if (!manager->threads)
		return (perror_alloc_create(0, "allocation issue in threads"));
	manager->forks = malloc(sizeof(pthread_mutex_t) * manager->n_philos);
	if (!manager->forks)
		return (perror_alloc_create(0, "allocation issue in forks"));
	nerror = pthread_mutex_init(&manager->printer, NULL);
	if (nerror)
		return (perror_alloc_create(nerror, "init mutex error in printer"));
	nerror = pthread_mutex_init(&manager->dead_mutex, NULL);
	if (nerror)
		return (perror_alloc_create(nerror, "init mutex error in dead_mutex"));
	nerror = pthread_mutex_init(&manager->satisfied, NULL);
	if (nerror)
		return (perror_alloc_create(nerror, "init mutex error in satisfied"));
	manager->philo_satisfied = 0;
	manager->dead = 0;
	return (0);
}

/*
	Main functioning:
	- Parsing the arguments and setting the values for the program.
	- Generating all the fork´s 'mutex' and the basic staff for 'philo' structs.
	- Generating all the 'threads'.
	- Creating the 'parca' that checks if someone hast to die.
	- Cleaning and closing the program when every 'thread' is done.
*/
int	main(int argc, char **argv)
{
	unsigned int	i;
	int				nerror;
	t_manager		manager;
	t_philo			**philo;

	nerror = parsing(argc, argv, &manager);
	if (nerror)
		return (nerror);
	nerror = init_main_vars(&manager, &philo, 0);
	if (nerror)
		return (nerror);
	nerror = create_philos(philo, &manager);
	if (nerror)
		return (nerror);
	i = 0;
	while (i < manager.n_philos)
	{
		nerror = pthread_create(&manager.threads[i],
				NULL, &thread_loop, philo[i]);
		if (nerror)
			return (perror_alloc_create(nerror, "thread creation error"));
		i++;
	}
	parca_loop(&manager, philo);
	return (clean_program(&manager, philo, 0));
}
