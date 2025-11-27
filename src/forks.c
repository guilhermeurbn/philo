/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/27 16:07:21 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 19:39:15 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void take_forks(t_philo *p)
{
    if (p->rules->n_philo == 1)
    {
        pthread_mutex_lock(p->left_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        return;
    }
    left_fork(p);
    right_fork(p);
}
void right_fork(t_philo *p)
{
    if (p->id % 2 == 0)
    {
        pthread_mutex_lock(p->right_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        if (is_dead(p->rules))
        {
            pthread_mutex_unlock(p->right_fork);
            return;
        }
        pthread_mutex_lock(p->left_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        if (is_dead(p->rules))
        {
            pthread_mutex_unlock(p->left_fork);
            pthread_mutex_unlock(p->right_fork);
        }
    }
}

void left_fork(t_philo *p)
{
    if (p->id % 2 != 0)
    {
        pthread_mutex_lock(p->left_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        if (is_dead(p->rules))
        {
            pthread_mutex_unlock(p->left_fork);
            return;
        }
        pthread_mutex_lock(p->right_fork);
        if (!is_dead(p->rules))
            print_action(p->rules, p->id, "has taken a fork");
        if (is_dead(p->rules))
        {
            pthread_mutex_unlock(p->right_fork);
            pthread_mutex_unlock(p->left_fork);
        }
    }
}

void drop_forks(t_philo *p)
{
    if (p->rules->n_philo == 1)
    {
        pthread_mutex_unlock(p->left_fork);
        return;
    }
    pthread_mutex_unlock(p->left_fork);
    pthread_mutex_unlock(p->right_fork);
}

void print_death(t_rules *rules, int id)
{
    pthread_mutex_lock(&rules->print_mutex);
    ft_putnbr_fd(get_time_in_ms() - rules->start_time, 1);
    ft_putchar_fd(' ', 1);
    ft_putnbr_fd(id, 1);
    ft_putstr_fd(" died\n", 1);
    pthread_mutex_unlock(&rules->print_mutex);
}
