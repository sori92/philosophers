/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_parse.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:45:37 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/17 12:29:56 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
	Ensuring the viability of the arguments and setting the general values
*/
int	parsing(int argc, char **argv, t_manager *manager)
{
	if (argc < 5 || argc > 6)
		return (perror_args("incorrect ammount of arguments"));
	manager->n_philos = ft_unsigned_atoi(argv[1]);
	manager->time_die = ft_unsigned_atoi(argv[2]);
	manager->time_eat = ft_unsigned_atoi(argv[3]);
	manager->time_sleep = ft_unsigned_atoi(argv[4]);
	if (!manager->n_philos || !manager->time_die
		|| !manager->time_eat || !manager->time_sleep)
		return (1);
	if (argc == 6)
		manager->goal_lunchs = ft_unsigned_atoi(argv[5]);
	printf("Gaol Lunchs: %u", manager->goal_lunchs);
	if (!manager->goal_lunchs)
		return (1);
	return (0);
}

int	perror_args(char *info)
{
	return (printf("error: %s\n", info), 1);
}

int	perror_alloc_create(int nerror, char *info)
{
	if (nerror)
		return (printf("error: %s\n", info), nerror);
	else
		return (printf("error: %s\n", info), 2);
}

int	perror_destroy(int nerror, char *info)
{
	return (printf("error %d: %s\n", nerror, info), nerror);
}
