/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:49:42 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 16:28:47 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void *monitor_thread(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_rules *rules = philos[0].rules;
    long now;
    int i;
    int all_ate;

    while (!is_dead(rules))
    {
        i = 0;
        all_ate = 1;
        while (i < rules->n_philo && !is_dead(rules))
        {
            pthread_mutex_lock(&philos[i].meal_mutex);
            now = get_time_in_ms();
            if ((now - philos[i].last_meal) >= rules->time_to_died)
            {
                pthread_mutex_unlock(&philos[i].meal_mutex);
                set_dead(rules);
                print_death(rules, philos[i].id);
                return NULL;
            }
            if (rules->must_eat > 0 && philos[i].meals_eaten < rules->must_eat)
                all_ate = 0;
            pthread_mutex_unlock(&philos[i].meal_mutex);
            i++;
        }
        if (rules->must_eat > 0 && all_ate && !is_dead(rules))
        {
            set_dead(rules);
            return NULL;
        }
        usleep(1000);
    }
    return NULL;
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
        printf("%ld %d %s\n", (get_time_in_ms() - rules->start_time), id, action);
    pthread_mutex_unlock(&rules->print_mutex);
}

