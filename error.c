/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:45:37 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/14 15:08:38 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	perror_args(char *info)
{
	printf("error: %s\n", info);
	return (1);
}

int	perror_alloc_create(int nerror, char *info)
{
	if (nerror)
	{
		printf("error %d: %s\n", nerror, info);
		return (nerror);
	}
	else
	{
		printf("error: %s\n", info);
		return (2);
	}
}

int	perror_destroy(int nerror, char *info)
{
	printf("error %d: %s\n", nerror, info);
	return (nerror);
}
