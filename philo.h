/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:09:56 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/12 12:39:30 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

#include "stdlib.h"
#include "stdio.h"
#include <sys/time.h>
#include <unistd.h>
#include <pthread.h>

typedef struct s_rules {
    int n_philo;
    int someone_died;
    long time_to_die;
    long time_to_eat;
    long time_to_sleep;
    long start_time;
    pthread_mutex_t death_mutex;
    pthread_mutex_t print_mutex;
    
} t_rules;

typedef struct s_philo {
    int id;
    long last_meal;
    int meals_eaten;
    pthread_t thread;
    pthread_mutex_t meal_mutex;
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_rules *rules;
} t_philo;




#endif