/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:45:37 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/08 19:35:28 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void    perror_args(char *info)
{
    printf("error: %s\n", info);
    exit (1);
}

void    perror_alloc_create(char *info)
{
    printf("error: %s\n", info);
    exit (2);
}

void    perror_destroy(char *info)
{
    printf("error: %s\n", info);
    exit (3);
}
