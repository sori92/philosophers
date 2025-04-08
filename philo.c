/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:29 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/08 13:50:14 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
    Ensuring the viability of the arguments and setting the general values
*/
void    parsing(int argc,  char **argv, t_manager *manager)
{

    if (argc < 4 && argc > 5)
        error_args("incorrect ammount of arguments");
    manager->n_philos = ft_unsigned_atoi(argv[1]);
    manager->time_die = ft_unsigned_atoi(argv[2]);
    manager->time_eat = ft_unsigned_atoi(argv[3]);
    manager->time_sleep = ft_unsigned_atoi(argv[4]);
    if (argc == 5)
        manager->goal_lunchs = ft_unsigned_atoi(argv[5]);
}

/*
    Destryoing all the mutex and threads and cleaning allocations,
    before closing the program.
*/
void    clean_philo(t_manager *manager)
{
    int i;

    i = 0;
    while (i < manager->n_philos)
    {
    pthread_mutex_destroy(manager->forks[i]);
    pthread_join(manager->threads[i], NULL);
    }
}

/*
    Make 'usleep' during the given time,
    but checking if someone is dead every 10 milisecs
*/
int    usleep_precise(unsigned int time, t_manager *manager)
{
    unsigned int    elapsed_time;

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
    Check the difference between a given 'start_time'
    and the current moment, returning it in milisecs.
*/
long   time_dif(struct timeval start_time)
{
    struct timeval current_time;

    gettimeofday(&current_time, NULL);
    return ((current_time.tv_sec - start_time.tv_sec) / 1000 +
        (current_time.tv_usec - start_time.tv_usec) * 1000);
}

/*
    Takes the forks and eats during the 'time_eat'.access
    If someone dies during the proccess manages to return.
*/
int    eat(t_philo philo, t_manager *manager)
{
    pthread_mutex_lock(manager->forks[philo.l_hand]);
    pthread_mutex_lock(manager->forks[philo.r_hand]);
    if (!usleep_precise(manager->time_eat, manager))
        return (0);
    pthread_mutex_unlock(manager->forks[philo.l_hand]);
    pthread_mutex_unlock(manager->forks[philo.r_hand]);
    return (1);
}

/*
    Main loop for the philo threads:
    It does eats and sleep, and manages to return if someone dies.
    Casting from a void is a mandatory from the 'pthread_create' function.
*/
void    thread_loop(void *input)
{
    t_philo         *philo;
    t_manager       *manager;
    struct timeval  start_time;

    philo = (t_philo *)input;
    manager = philo->manager;
    gettimeofday(&start_time, NULL);
    while (1)
    {
        if (!eat(*philo, manager))
            return ;
        if (!usleep_precise(manager->time_sleep, manager))
            return ;
    }
}

/*
    Loop that is always checking if someone has to die from starvation.
*/
void    parca_loop(t_manager *manager, t_philo **philo)
{
    int i;

    while (manager->dead == 0)
    {
        i = 0;
        while (philo[i])
        {
            if (time_dif(philo[i]->last_lunch) > manager->time_die)
            {
                manager->dead = 1;
                break ;
            }
            i++;
        }
    }
}

/*
    Main functioning:
    - Parsing the arguments and setting the values for the program.
    - Generating all the philo 'threads' and the fork´s 'mutex'.
    - Creating the 'parca' that checks if someone hast to die.
    - Cleaning and closing the program when every 'thread' is done.
*/
int main(int argc,  char **argv)
{
    int         i;
    t_manager   manager;
    t_philo     **philo;

    parsing(argc, argv, &manager);
    philo = malloc(sizeof(t_philo *) * manager.n_philos);
    if (!philo)
        error_alloc_create("allocation issue in pthread_t **philo");
    manager.threads = malloc(sizeof(pthread_t *) * manager.n_philos);
    if (!manager.threads)
        error_alloc_create("allocation issue in pthread_t **threads");
    i = 0;
    while (i < manager.n_philos)
    {
        /* manager.threads[i] = malloc(sizeof(pthread_t));
        if (!manager.threads[i])
            error_alloc_create("allocation issue in pthread_t *thread"); */
        pthread_mutex_init(manager.forks[i], NULL);
        philo[i] = malloc(sizeof(t_philo));
        if (!philo[i])
            error_alloc_create("allocation issue in pthread_t *philo");
        philo[i]->name = i;
        pthread_create(manager.threads[i], thread_loop, philo[i]);
        i++;
    }
    parca_loop(&manager, philo);
    clean_philo(&manager);
}
