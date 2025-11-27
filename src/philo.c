/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:32:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 18:37:02 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int main(int argc, char **argv)
{
    t_rules rules;
    t_philo philos[200];
    pthread_t monitor;
    pthread_mutex_t forks[200];
    
    
    int i;
    int n;

    if (init_rules(&rules, argc, argv))
        return (1);
    
    if (init_philos(philos, forks, &rules))
    {
        destroy_mutex(philos, forks, &rules, rules.n_philo);
        return (1);
    }
    
    n = rules.n_philo;

    i = 0;
    while (i < n)
    {
        if (pthread_create(&philos[i].thread, NULL, philos_routine, &philos[i]))
        {
            ft_putstr_fd("Error: Failed to create thread\n", 2);
            destroy_mutex(philos, forks, &rules, n);
            return (1);
        }
        i++;
    }
    if (pthread_create(&monitor, NULL, monitor_thread, philos))
    {
        ft_putstr_fd("Error: Failed to create monitor thread\n", 2);
        destroy_mutex(philos, forks, &rules, n);
        return (1);
    }
    i = 0;
    while (i < n)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    pthread_join(monitor, NULL);
    
    destroy_mutex(philos, forks, &rules, n);
    return (0);
}