/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:29 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/07 19:00:00 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

parsing(int argc,  char **argv, t_manager *manager)
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

clean_philo(t_manager *manager)
{
    int i;

    i = 0;
    while (i < manager->n_philos)
    {
    pthread_mutex_destroy(manager->forks[i]);
    pthread_join(manager->threads[i], NULL);
    }
}

void    usleep_precise(unsigned int time)
{
    unsigned int    elapsed_time;

    elapsed_time = 0;
    while (elapsed_time < time)
    {
        usleep(10);
        elapsed_time += 10;
    }
}

long   time_dif(struct timeval start_time)
{
    struct timeval current_time;

    gettimeofday(&current_time, NULL);
    return ((current_time.tv_sec - start_time.tv_sec) / 1000 +
        (current_time.tv_usec - start_time.tv_usec) * 1000);
}

void    eat(t_philo philo, t_manager *manager)
{
    pthread_mutex_lock(manager->forks[philo.l_hand]);
    pthread_mutex_lock(manager->forks[philo.r_hand]);
    usleep_precise(manager->time_eat);
    pthread_mutex_unlock(manager->forks[philo.l_hand]);
    pthread_mutex_unlock(manager->forks[philo.r_hand]);
}

thread_loop(void *input)
{
    t_philo         *philo;
    t_manager       *manager;
    struct timeval  start_time;

    philo = (t_philo *)input;
    manager = philo->manager;
    gettimeofday(&start_time, NULL);
    while (1)
    {
        eat(*philo, manager);
        usleep_precise(manager->time_sleep);
    }
}

parca_loop(t_manager *manager)
{
    while (manager->dead == 0)
    {
        
    }
    
}

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
    parca_loop(&manager);
    clean_philo(&manager);
}
