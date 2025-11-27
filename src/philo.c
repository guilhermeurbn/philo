/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:32:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 16:39:02 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int validate_arguments(int argc, char **argv)
{
    int i;
    long val;

    if (argc != 5 && argc != 6)
    {
        printf("Ex: ./philo n_philo time_to_died time_to_eat time_to_sleep [must_eat]\n");
        return (1);
    }
    i = 1;
    while (i < argc)
    {
        val = atol(argv[i]);
        if (val <= 0 || (i == 1 && val > 200))
        {
            printf("Error: Invalid arguments\n");
            return (1);
        }
        i++;
    }
    return (0);
}

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
            printf("Error: Failed to create thread\n");
            destroy_mutex(philos, forks, &rules, n);
            return (1);
        }
        i++;
    }

    if (pthread_create(&monitor, NULL, monitor_thread, philos))
    {
        printf("Error: Failed to create monitor thread\n");
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