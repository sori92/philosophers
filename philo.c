/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:29 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/14 19:23:31 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Ensuring the viability of the arguments and setting the general values
*/
static int	parsing(int argc, char **argv, t_manager *manager)
{
	if (argc < 5 || argc > 6)
		return (perror_args("incorrect ammount of arguments"));
	manager->n_philos = ft_unsigned_atoi(argv[1]);
	manager->time_die = ft_unsigned_atoi(argv[2]);
	manager->time_eat = ft_unsigned_atoi(argv[3]);
	manager->time_sleep = ft_unsigned_atoi(argv[4]);
	if (!manager->n_philos || !manager->n_philos || !manager->n_philos || !manager->n_philos)
		return (1);
	if (argc == 6)
		manager->goal_lunchs = ft_unsigned_atoi(argv[5]);
	return (0);
}

/*
	Destryoing all the mutex and threads and cleaning allocations,
	before closing the program.
*/
static int	clean_philo(t_manager *manager, t_philo **philo, int nerror, unsigned int i)
{
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
	i = 0;
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
	return (0);
}

/*
	Loop that is always checking if someone has to die from starvation.
	The moment that´s considered for starting the starvation
	is the moment of starting to eat (yes, I know it has no much sense,
	but that´s how it´s supposed to work).
*/
static void	parca_loop(t_manager *manager, t_philo **philo)
{
	unsigned int	i;
	long			dif;

	while (!manager->start_program)
		usleep(10);
	while (manager->dead == 0)
	{
		i = 0;
		while (i < manager->n_philos)
		{
			pthread_mutex_lock(&philo[i]->lunch_mutex);
			dif = time_dif(philo[i]->last_lunch);
			pthread_mutex_unlock(&philo[i]->lunch_mutex);
			if (dif > manager->time_die)
			{
				printf("MUERTE POR QUE HA PASADO ESTE TIEMPO: %ld\n", dif);
				//pthread_mutex_lock(&manager->dead_mutex);
				manager->dead = 1;
				//pthread_mutex_unlock(&manager->dead_mutex);
				state_printer(BG_RED, philo[i], "died");
				break ;
			}
			i++;
		}
		if (manager->philo_satisfed == manager->n_philos)
			break;
	}
}

/*
    Initiating some basic main values:
    - Allocation of space for 'philo**', 'threads*' and 'forks*'.
	- Creation of a 'mutex' for the prints.
	- Setting the 'dead' to 0.
*/
static int	init_main_vars(t_manager *manager, t_philo ***philo, int nerror)
{
	*philo = malloc(sizeof(t_philo *) * manager->n_philos);
	if (!(*philo))
		return (perror_alloc_create(0, "allocation issue in pthread_t philo"));
	manager->threads = malloc(sizeof(pthread_t) * manager->n_philos);
	if (!manager->threads)
		return (perror_alloc_create(0, "allocation issue in pthread_t threads"));
	manager->forks = malloc(sizeof(pthread_mutex_t) * manager->n_philos);
	if (!manager->forks)
		return (perror_alloc_create(0, "allocation issue in forks"));
	nerror = pthread_mutex_init(&manager->printer, NULL);
	if (nerror)
		return (perror_alloc_create(nerror, "init mutex error in printer"));
	manager->philo_satisfed = 0;
	manager->dead = 0;
	manager->start_program = 0;
	return (0);
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

	if ((nerror = parsing(argc, argv, &manager)))
		return (nerror);
	if ((nerror = init_main_vars(&manager, &philo, 0)))
		return (nerror);
	gettimeofday(&manager.start_time, NULL);
	i = 0;
	while (i < manager.n_philos)
	{
		nerror = pthread_mutex_init(&manager.forks[i], NULL);
		if (nerror)
			return (perror_alloc_create(nerror, "init mutex error in forks"));
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			return (perror_alloc_create(0, "allocation issue in pthread_t *philo"));
		philo[i]->name = i + 1;
		philo[i]->manager = &manager;
		i++;
	}
	i = 0;
	while (i < manager.n_philos)
	{
		nerror = pthread_create(&manager.threads[i], NULL, &thread_loop, philo[i]);
		if (nerror)
			return (perror_alloc_create(nerror, "thread creation error"));
		i++;
	}
	usleep(1000000);
	//gettimeofday(&manager.start_time, NULL);
	manager.start_program = 1;
	parca_loop(&manager, philo);
	if ((nerror = clean_philo(&manager, philo, 0, 0)))
		return (nerror);
	return (0);
}
