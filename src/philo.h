/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:09:56 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 18:10:37 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/time.h>
#include "../library/libft/libft.h"

typedef struct s_rules 
{
    int n_philo;
    int must_eat;
    int someone_died;
    long time_to_died;
    long time_to_eat;
    long time_to_sleep;
    long start_time;
    pthread_mutex_t death_mutex;
    pthread_mutex_t print_mutex;
    
}   t_rules;

typedef struct s_philo 
{
    int id;
    long last_meal;
    int meals_eaten;
    pthread_t thread;
    pthread_mutex_t meal_mutex;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_rules *rules;
}   t_philo;

void    destroy_mutex(t_philo *philos, pthread_mutex_t *forks, t_rules *rules, int n);
void    take_forks(t_philo *p);
void    drop_forks(t_philo *p);
void    print_death(t_rules *rules, int id);
void    *monitor_thread(void *arg);
void    set_dead(t_rules *rules);
void    print_action(t_rules *rules, int id, char *action);
void    *philos_routine(void *arg);
void    *single_philo_routine(void *arg);
void    smart_sleep(long ms, t_rules *rules);
long    get_time_in_ms(void);
int     is_dead(t_rules *rules);
int     validate_arguments(int argc, char **argv);
int     init_rules(t_rules *rules, int argc, char **argv);
int     init_philos(t_philo *philos, pthread_mutex_t *forks, t_rules *rules);
    
#endif