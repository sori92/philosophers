/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:29 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/10 20:07:34 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Ensuring the viability of the arguments and setting the general values
*/
static void	parsing(int argc, char **argv, t_manager *manager)
{
	if (argc < 5 || argc > 6)
		perror_args("incorrect ammount of arguments");
	manager->n_philos = ft_unsigned_atoi(argv[1]);
	manager->time_die = ft_unsigned_atoi(argv[2]);
	manager->time_eat = ft_unsigned_atoi(argv[3]);
	manager->time_sleep = ft_unsigned_atoi(argv[4]);
	if (argc == 6)
		manager->goal_lunchs = ft_unsigned_atoi(argv[5]);
}

/*
	Destryoing all the mutex and threads and cleaning allocations,
	before closing the program.
*/
static void	clean_philo(t_manager *manager, int nerror, unsigned int i)
{
	while (i < manager->n_philos)
	{
		if (manager->threads[i])
		{
			nerror = pthread_join(manager->threads[i], NULL);
			if (nerror)
				perror_destroy(nerror, "join error in threads");
		}
		i++;
	}
	i = 0;
	while (i < manager->n_philos)
	{
		if (manager->forks)
		{
			nerror = pthread_mutex_destroy(&manager->forks[i]);
			if (nerror)
				perror_destroy(nerror, "destroy mutex error in forks");
		}
		i++;
	}
	nerror = pthread_mutex_destroy(&manager->printer);
	if (nerror)
		perror_destroy(nerror, "destroy mutex error in printer");
}

/*
	Loop that is always checking if someone has to die from starvation.
	The moment that´s considered for starting the starvation
	is the moment of starting to eat (yes, I know it has no much sense,
	but that´s how it´s supposed to work).
*/
static void	parca_loop(t_manager *manager, t_philo **philo)
{
	int	i;

	while (manager->dead == 0)
	{
		i = 0;
		while (philo[i])
		{
			if (time_dif(philo[i]->last_lunch) > manager->time_die)
			{
				manager->dead = 1;
				state_printer(BG_RED, philo[i], "died");
				break ;
			}
			i++;
		}
	}
}

static void	init_main_vars(t_manager *manager, t_philo ***philo, int nerror)
{
	*philo = malloc(sizeof(t_philo *) * manager->n_philos);
	if (!(*philo))
		perror_alloc_create(0, "allocation issue in pthread_t philo");
	manager->threads = malloc(sizeof(pthread_t) * manager->n_philos);
	if (!manager->threads)
		perror_alloc_create(0, "allocation issue in pthread_t threads");
	manager->forks = malloc(sizeof(pthread_mutex_t) * manager->n_philos);
	if (!manager->forks)
		perror_alloc_create(0, "allocation issue in forks");
	nerror = pthread_mutex_init(&manager->printer, NULL);
	if (nerror)
		perror_alloc_create(nerror, "init mutex error in printer");
	manager->dead = 0;
}

/*
	Main functioning:
	- Parsing the arguments and setting the values for the program.
	- Generating all the philo 'threads' and the fork´s 'mutex'.
	- Creating the 'parca' that checks if someone hast to die.
	- Cleaning and closing the program when every 'thread' is done.
*/
int	main(int argc, char **argv)
{
	unsigned int	i;
	int				nerror;
	t_manager		manager;
	t_philo			**philo;

	parsing(argc, argv, &manager);
	init_main_vars(&manager, &philo, 0);
	i = 0;
	while (i < manager.n_philos)
	{
		nerror = pthread_mutex_init(&manager.forks[i], NULL);
		if (nerror)
			perror_alloc_create(nerror, "init mutex error in forks");
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			perror_alloc_create(0, "allocation issue in pthread_t *philo");
		philo[i]->name = i + 1;
		philo[i]->manager = &manager;
		pthread_create(&manager.threads[i], NULL, &thread_loop, philo[i]);
		i++;
	}
	sleep(1);
	parca_loop(&manager, philo);
	clean_philo(&manager, 0, 0);
	exit(0);
}
