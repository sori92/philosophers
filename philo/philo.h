/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:27:22 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/21 19:29:04 by dsoriano         ###   ########.fr       */
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
	unsigned int	philo_satisfied;
	unsigned int	dead;
	pthread_mutex_t	printer;
	pthread_mutex_t	*forks;
	pthread_mutex_t	dead_mutex;
	pthread_mutex_t	satisfied;
	pthread_t		*threads;
}	t_manager;

typedef struct s_philo
{
	unsigned int	name;
	unsigned int	r_hand;
	unsigned int	l_hand;
	unsigned int	die_time;
	struct timeval	start_time;
	unsigned int	lunch_count;
	unsigned int	count_reached;
	struct timeval	last_lunch;
	pthread_mutex_t	lunch_mutex;
	t_manager		*manager;
}	t_philo;

	//THREADS
void			*thread_loop(void *input);
void			state_printer(const char *color,
					t_philo *philo, const char *str);
int				check_dead(pthread_mutex_t *mutex0,
					pthread_mutex_t *mutex1, t_manager *manager);

	//EAT LOOP
int				eat(t_philo *philo, t_manager *manager);

	//TIME MANAGEMENT
int				usleep_precise(unsigned long time, t_manager *manager);
unsigned long	my_get_time_stamp(void);
unsigned int	time_dif(struct timeval start_tv);

	//AUXILIARS
int				is_even(unsigned int name);
unsigned int	ft_unsigned_atoi(const char *str);
unsigned int	ft_unsigned_itoa(unsigned int n, char *str);

	//ERROR HANDLING AND PARSING
int				parsing(int argc, char **argv, t_manager *manager);
int				perror_args(char *info);
int				perror_alloc_create(int nerror, char *info);
int				perror_destroy(int nerror, char *info);

	//DEAD FUNCTIONALITIES
void			parca_loop(t_manager *manager, t_philo **philo);

#endif
