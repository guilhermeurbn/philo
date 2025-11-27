/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:07:21 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 16:07:29 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void take_forks(t_philo *p)
{
    if (p->id % 2 == 0)
    {
        pthread_mutex_lock(p->right_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        else
            pthread_mutex_unlock(p->right_fork);
        pthread_mutex_lock(p->left_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        else
            pthread_mutex_unlock(p->left_fork);
    }
    else
    {
        pthread_mutex_lock(p->left_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        else
            pthread_mutex_unlock(p->left_fork);
        pthread_mutex_lock(p->right_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        else
            pthread_mutex_unlock(p->right_fork);
    }
}

void drop_forks(t_philo *p)
{
    pthread_mutex_unlock(p->left_fork);
    pthread_mutex_unlock(p->right_fork);
}

void print_death(t_rules *rules, int id)
{
    pthread_mutex_lock(&rules->print_mutex);
    printf("%ld %d died\n", (get_time_in_ms() - rules->start_time), id);
    pthread_mutex_unlock(&rules->print_mutex);
}
