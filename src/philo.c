/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:32:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/28 16:08:25 by guisanto         ###   ########.fr       */
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
        return (destroy_mutex(philos, forks, &rules, rules.n_philo), 1);
    n = rules.n_philo;
    if (start_philos_threads(philos, &rules, forks))
        return (1);
    if (pthread_create(&monitor, NULL, monitor_thread, philos))
    {
        ft_putstr_fd("Error: Failed to create monitor thread\n", 2);
        return (destroy_mutex(philos, forks, &rules, n), 1);
    }
    i = -1;    
    while (++i < n)
        pthread_join(philos[i].thread, NULL);
    pthread_join(monitor, NULL);
    destroy_mutex(philos, forks, &rules, n);
    return (0);
}

