/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:24:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 16:24:46 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *single_philo_routine(void *arg)
{
    t_philo *p = (t_philo *)arg;
    t_rules *rules = p->rules;

    print_action(rules, p->id, "has taken a fork");
    smart_sleep(rules->time_to_died, rules);
    return NULL;
}

void *philos_routine(void *arg)
{
    t_philo *p;
    t_rules *rules;

    p = (t_philo *)arg;
    rules = p->rules;

    if (rules->n_philo == 1)
        return single_philo_routine(arg);

    if (p->id % 2 == 0)
        usleep(1000);

    while (!is_dead(rules))
    {
        take_forks(p);
        if (is_dead(rules))
        {
            drop_forks(p);
            break;
        }
        
        pthread_mutex_lock(&p->meal_mutex);
        p->last_meal = get_time_in_ms();
        p->meals_eaten++;
        pthread_mutex_unlock(&p->meal_mutex);
        
        print_action(rules, p->id, "is eating");
        smart_sleep(rules->time_to_eat, rules);
        
        drop_forks(p);
        
        if (is_dead(rules))
            break;
            
        print_action(rules, p->id, "is sleeping");
        smart_sleep(rules->time_to_sleep, rules);
            
        if (is_dead(rules))
            break;
            
        print_action(rules, p->id, "is thinking");
        
        if (rules->n_philo % 2 == 1)
            usleep(500);
    }
    return (NULL);
}