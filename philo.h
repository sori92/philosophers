/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:22 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/13 20:39:59 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

	//LIBRARIES
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <limits.h>
# include <sys/time.h>

	//PRINT COLORS
# define RESET		"\x1b[0m"
# define RED		"\x1b[31m"
# define BROWN		"\x1b[38;5;94m" 
# define ORANGE		"\x1b[38;5;214m"
# define GREEN		"\x1b[32m"
# define YELLOW		"\x1b[33m"
# define BLUE		"\x1b[34m"
# define MAGENTA	"\x1b[35m"
# define CYAN		"\x1b[36m"
# define WHITE		"\x1b[37m"

	// Background colors
# define BG_RED		"\x1b[41m"
# define BG_BROWN	"\x1b[48;5;94m"
# define BG_ORANGE	"\x1b[48;5;214m"
# define BG_GREEN	"\x1b[42m"
# define BG_YELLOW	"\x1b[43m"
# define BG_BLUE	"\x1b[44m"
# define BG_MAGENTA	"\x1b[45m"
# define BG_CYAN	"\x1b[46m"
# define BG_WHITE	"\x1b[47m"

typedef struct s_manager
{
	unsigned int	n_philos;
	unsigned int	time_die;
	unsigned int	time_eat;
	unsigned int	time_sleep;
	unsigned int	goal_lunchs;
	unsigned int	dead;
	pthread_mutex_t	printer;
	pthread_mutex_t	*forks;
	pthread_t		*threads;
}	t_manager;

typedef struct s_philo
{
	unsigned int	name;
	unsigned int	r_hand;
	unsigned int	l_hand;
	struct timeval	last_lunch;
	struct timeval	start_time;
	t_manager		*manager;
}	t_philo;

	//THREADS
void			*thread_loop(void *input);
void			state_printer(const char *color,
					t_philo *philo, const char *str);

	//AUXILIARS
int				is_even(unsigned int name);
long			time_dif(struct timeval start_tv);
unsigned int	ft_unsigned_atoi(const char *str);

	//ERROR HANDLING
void			perror_args(char *info);
void			perror_alloc_create(int nerror, char *info);
void			perror_destroy(int nerror, char *info);

	//TESTING FUNCTIONALITIES
void			show_manager(t_manager manager);
void			show_philo(t_philo philo);

#endif
