/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:49:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 16:23:52 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_rules(t_rules *rules, int argc, char **argv)
{
    if (validate_arguments(argc, argv))
        return (1);
    
    rules->n_philo = atoi(argv[1]);
    rules->time_to_died = atol(argv[2]);
    rules->time_to_eat = atol(argv[3]);
    rules->time_to_sleep = atol(argv[4]);
    if (argc == 6)
        rules->must_eat = atoi(argv[5]);
    else
        rules->must_eat = -1;
    rules->someone_died = 0;
    rules->start_time = get_time_in_ms();
    
    if (pthread_mutex_init(&rules->death_mutex, NULL) ||
        pthread_mutex_init(&rules->print_mutex, NULL))
    {
        printf("Error: Failed to initialize mutexes\n");
        return (1);
    }
    return (0);
}

int init_philos(t_philo *philos, pthread_mutex_t *forks, t_rules *rules)
{
    int i;
    int n;
    
    i = 0;
    n = rules->n_philo;

    while(i < n)
    {
        if (pthread_mutex_init(&forks[i], NULL) ||
            pthread_mutex_init(&philos[i].meal_mutex, NULL))
        {
            printf("Error: Failed to initialize mutexes\n");
            return (1);
        }
        philos[i].id = i + 1;
        philos[i].rules = rules;
        philos[i].last_meal = rules->start_time;
        philos[i].meals_eaten = 0;
        philos[i].left_fork = &forks[i];
        philos[i].right_fork = &forks[(i + 1) % n];
        i++;
    }
    return (0);
}