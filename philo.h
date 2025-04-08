/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:22 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/08 13:50:14 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>


typedef struct s_manager
{
    unsigned int    n_philos;
    unsigned int    time_die;
    unsigned int    time_eat;
    unsigned int    time_sleep;
    unsigned int    goal_lunchs;
    unsigned int    dead;
    pthread_mutex_t **forks;
    pthread_t       **threads;
}	t_manager;

typedef struct s_philo
{
    unsigned int    name;
    unsigned int    r_hand;
    unsigned int    l_hand;
    unsigned int    last_lunch;
	t_manager       *manager;
}	t_philo;

    //PARSING
    unsigned int	ft_unsigned_atoi(const char *str);

    //ERROR HANDLING
    error_args(char *info);
    error_alloc_create(char *info);

#endif
