/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dsoriano <dsoriano@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/07 13:45:37 by dsoriano          #+#    #+#             */
/*   Updated: 2025/04/07 15:11:33 by dsoriano         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

error_args(char *info)
{
    printf("error: %s\n", info);
    exit (1);
}

error_alloc_create(char *info)
{
    printf("error: %s\n", info);
    exit (2);
}
