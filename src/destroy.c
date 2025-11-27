/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:49:31 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 19:26:08 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void destroy_mutex(t_philo *philos, pthread_mutex_t *forks, t_rules *rules, int n)
{
    int i;

    i = 0;
    while (i < n)
    {
        pthread_mutex_destroy(&forks[i]);
        pthread_mutex_destroy(&philos[i].meal_mutex);
        i++;
    }
    pthread_mutex_destroy(&rules->death_mutex);
    pthread_mutex_destroy(&rules->print_mutex);
}
