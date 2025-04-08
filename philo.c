/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:29 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/08 19:38:38 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
    Ensuring the viability of the arguments and setting the general values
*/
void    parsing(int argc,  char **argv, t_manager *manager)
{

    if (argc < 4 || argc > 5)
        perror_args("incorrect ammount of arguments");
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
    unsigned int    i;

    i = 0;
    while (i < manager->n_philos)
    {
        if (manager->forks && &manager->forks[i])
            if (pthread_mutex_destroy(&manager->forks[i]))
                perror_destroy("destroy mutex error in forks");
        i++;
    }
    i = 0;
    while (i < manager->n_philos)
    {
        if (manager->threads[i])
            if (pthread_join(manager->threads[i], NULL))
                perror_destroy("join error in threads");
        i++;
    }
    if (pthread_mutex_destroy(&manager->printer))
        perror_destroy("join error in tprinter");
    exit(0);
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
    Check the difference between a given moment
    and the current moment, returning it in milisecs.
*/
long   time_dif(struct timeval start_tv)
{
    struct timeval  current_tv;
    unsigned int    start_time;
    unsigned int    current_time;

    gettimeofday(&current_tv, NULL);
    start_time = start_tv.tv_sec / 1000 + start_tv.tv_usec * 1000;
    current_time = current_tv.tv_sec / 1000 + current_tv.tv_usec * 1000;
    return (current_time - start_time);
}

/*
    Printing name and state of the philo, according to the selected color.
*/
void state_printer(const char *color, t_philo *philo, const char *str)
{
    struct timeval  current_tv;
    unsigned int    current_time; 

    gettimeofday(&current_tv, NULL);
    current_time = current_tv.tv_sec / 1000 + current_tv.tv_usec * 1000;
    pthread_mutex_lock(&philo->manager->printer);
    printf("%s%d %d %s\x1b[0m\n", color, current_time, philo->name, str);
    pthread_mutex_unlock(&philo->manager->printer);
}

/*
    Takes the forks and eats during the 'time_eat'.
    If someone dies during the proccess manages to return.
*/
int    eat(t_philo *philo, t_manager *manager)
{
    pthread_mutex_lock(&manager->forks[philo->l_hand]);
    state_printer(BG_YELLOW, philo, "has taken a fork");
    pthread_mutex_lock(&manager->forks[philo->r_hand]);
    state_printer(BG_ORANGE, philo, "has taken a fork");
    gettimeofday(&philo->last_lunch, NULL);
    state_printer(BG_BROWN, philo, "is eating");
    if (!usleep_precise(manager->time_eat, manager))
        return (0);
    pthread_mutex_unlock(&manager->forks[philo->l_hand]);
    pthread_mutex_unlock(&manager->forks[philo->r_hand]);
    return (1);
}

/*
    Initiating some basic philo values.
    The forks are shared in a closed circle, this way:
    - The left hand always has the fork of philo name -1
    (forks start in 0 and names start in 1).
    - The right hand has the fork of philo name,
    except for the last philo, who has the fork 0.
*/
void    init_philo_vars( t_philo *philo,t_manager manager)
{
    gettimeofday(&philo->start_time, NULL);
    philo->l_hand = philo->name - 1;
    if (philo->name == manager.n_philos)
        philo->r_hand = 0;
    else
        philo->r_hand = philo->name;
}

/*
    Main loop for the philo threads:
    It does eats and sleep, and manages to return if someone dies.
    Casting from a void is a mandatory from the 'pthread_create' function.
*/
void    *thread_loop(void *input)
{
    t_philo         *philo;
    t_manager       *manager;

    philo = (t_philo *)input;
    manager = philo->manager;
    init_philo_vars(philo, *manager);
    show_philo(*philo);
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

/*
    Loop that is always checking if someone has to die from starvation.
    The moment that´s considered for starting the starvation is the moment of starting to eat
    (yes, I know it has no much sense, but that´s how it´s supposed to work).
*/
void    parca_loop(t_manager *manager, t_philo **philo)
{
    int             i;
    struct timeval  current_tv;
    unsigned int    current_time;

    while (manager->dead == 0)
    {
        i = 0;
        while (philo[i])
        {
            if (time_dif(philo[i]->last_lunch) > manager->time_die)
            {
                manager->dead = 1;
                gettimeofday(&current_tv, NULL);
                current_time = current_tv.tv_sec / 1000 + current_tv.tv_usec * 1000;
                state_printer(BG_RED, philo[i], "died");
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
    unsigned int    i;
    t_manager       manager;
    t_philo         **philo;

    parsing(argc, argv, &manager);
    philo = malloc(sizeof(t_philo *) * manager.n_philos);
    if (!philo)
        perror_alloc_create("allocation issue in pthread_t *philo");
    manager.threads = malloc(sizeof(pthread_t) * manager.n_philos);
    if (!manager.threads)
        perror_alloc_create("allocation issue in pthread_t threads");
    manager.forks = malloc(sizeof(pthread_mutex_t) * manager.n_philos);
    if (!manager.forks)
        perror_alloc_create("allocation issue in forks");
    pthread_mutex_init(&manager.printer, NULL);
    show_manager(manager);
    i = 0;
    while (i < manager.n_philos)
    {
        pthread_mutex_init(&manager.forks[i], NULL);
        philo[i] = malloc(sizeof(t_philo));
        if (!philo[i])
            perror_alloc_create("allocation issue in pthread_t *philo");
        philo[i]->name = i + 1;
        philo[i]->manager = &manager;
        pthread_create(&manager.threads[i], NULL, &thread_loop, philo[i]);
        i++;
    }
    parca_loop(&manager, philo);
    clean_philo(&manager);
}
