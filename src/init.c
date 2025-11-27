/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:49:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 19:41:49 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int init_rules(t_rules *rules, int argc, char **argv)
{
    if (validate_arguments(argc, argv))
        return (1);

    rules->n_philo = ft_atoi(argv[1]);
    rules->time_to_died = ft_atol(argv[2]);
    rules->time_to_eat = ft_atol(argv[3]);
    rules->time_to_sleep = ft_atol(argv[4]);
    if (argc == 6)
        rules->must_eat = ft_atoi(argv[5]);
    else
        rules->must_eat = -1;
    rules->someone_died = 0;
    rules->start_time = get_time_in_ms();
    
    if (pthread_mutex_init(&rules->death_mutex, NULL) ||
        pthread_mutex_init(&rules->print_mutex, NULL))
    {
        ft_putstr_fd("Error: Failed to initialize mutexes\n", 2);
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
            ft_putstr_fd("Error: Failed to initialize mutexes\n", 2);
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
int start_philos_threads(t_philo *philos, t_rules *rules, pthread_mutex_t *forks)
{
    int i;
    int n;

    n = rules->n_philo;
    i = 0;
    while (i < n)
    {
        if (pthread_create(&philos[i].thread, NULL, philos_routine, &philos[i]))
        {
            ft_putstr_fd("Error: Failed to create thread\n", 2);
            destroy_mutex(philos, forks, rules, n);
            return (1);
        }
        i++;
    }
    return (0);
}

int validate_arguments(int argc, char **argv)
{
    int i;
    long val;

    if (argc != 5 && argc != 6)
    {
        ft_putstr_fd("Ex: ./philo n_philo time_to_died time_to_eat time_to_sleep [must_eat]\n", 2);
        return (1);
    }
    i = 1;
    while (i < argc)
    {
        val = ft_atol(argv[i]);
        if (val <= 0 || (i == 1 && val > 200))
        {
            ft_putstr_fd("Error: Invalid arguments\n", 2);
            return (1);
        }
        i++;
    }
    return (0);
}
