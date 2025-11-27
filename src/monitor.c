/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:49:42 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 19:07:18 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *monitor_thread(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_rules *rules = philos[0].rules;
    long now;
    int i;

    now = 0;
    while (!is_dead(rules))
    {
        i = 0;
        while (i < rules->n_philo && !is_dead(rules))
        {
            if (check_death(&philos[i], rules, now))
                return (NULL);
            i++;
        }
        if (check_all_ate(philos, rules))
            return (NULL);
        usleep(1000);
    }
    return (NULL);
}

int check_death(t_philo *philo, t_rules *rules, long now)
{
    pthread_mutex_lock(&philo->meal_mutex);
    now = get_time_in_ms();
    if ((now - philo->last_meal) >= rules->time_to_died)
    {
        pthread_mutex_unlock(&philo->meal_mutex);
        set_dead(rules);
        print_death(rules, philo->id);
        return (1);
    }
    pthread_mutex_unlock(&philo->meal_mutex);
    return (0);
}

int check_all_ate(t_philo *philos, t_rules *rules)
{
    int i;
    int all_ate;

    i = 0;
    all_ate = 1;
    while (i < rules->n_philo)
    {
        pthread_mutex_lock(&philos[i].meal_mutex);
        if (rules->must_eat > 0 && philos[i].meals_eaten < rules->must_eat)
            all_ate = 0;
        pthread_mutex_unlock(&philos[i].meal_mutex);
        i++;
    }
    if (rules->must_eat > 0 && all_ate)
    {
        set_dead(rules);
        return (1);
    }
    return (0);
}

int is_dead(t_rules *rules)
{
    int val;
    pthread_mutex_lock(&rules->death_mutex);
    val = rules->someone_died;
    pthread_mutex_unlock(&rules->death_mutex);
    return (val);
}

void set_dead(t_rules *rules)
{
    pthread_mutex_lock(&rules->death_mutex);
    rules->someone_died = 1;
    pthread_mutex_unlock(&rules->death_mutex);
}

void print_action(t_rules *rules, int id, char *action)
{
    pthread_mutex_lock(&rules->print_mutex);
    if (!is_dead(rules))
    {
        ft_putnbr_fd(get_time_in_ms() - rules->start_time, 1);
        ft_putchar_fd(' ', 1);
        ft_putnbr_fd(id, 1);
        ft_putstr_fd(" ", 1);
        ft_putstr_fd(action, 1);
        ft_putchar_fd('\n', 1);
    }
    pthread_mutex_unlock(&rules->print_mutex);
}

