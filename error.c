/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:45:37 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/10 19:31:57 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	perror_args(char *info)
{
	printf("error: %s\n", info);
	exit (1);
}

void	perror_alloc_create(int nerror, char *info)
{
	if (nerror)
	{
		printf("error %d: %s\n", nerror, info);
		exit (nerror);
	}
	else
	{
		printf("error: %s\n", info);
		exit (2);
	}
}

void	perror_destroy(int nerror, char *info)
{
	printf("error %d: %s\n", nerror, info);
	exit (nerror);
}
