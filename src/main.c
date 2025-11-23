/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:32:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/22 18:03:07 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long get_time_in_ms(void)
{
    struct timeval tv;
    long    ms;

    gettimeofday(&tv, NULL);
    ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (ms);
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

void smart_sleep(long ms, t_rules *rules)
{
    long start = get_time_in_ms();

    while (!is_dead(rules))
    {
        if (get_time_in_ms() - start >= ms)
            break;
        usleep(100);
    }
}

void print_action(t_rules *rules, int id, char *action)
{
    pthread_mutex_lock(&rules->print_mutex);
    if (!is_dead(rules))
        printf("%ld %d %s\n", (get_time_in_ms() - rules->start_time), id, action);
    pthread_mutex_unlock(&rules->print_mutex);
}

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

void cleanup_resources(t_philo *philos, pthread_mutex_t *forks, t_rules *rules, int n)
{
    int i;

    i = 0;
    while (i < n)
    {
        pthread_mutex_destroy(&forks[i]);
        pthread_mutex_destroy(&philos[i].meal_mutex);
        i++;
    }
    pthread_mutex_destroy(&rules->death_mutex);
    pthread_mutex_destroy(&rules->print_mutex);
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
        cleanup_resources(philos, forks, &rules, rules.n_philo);
        return (1);
    }
    
    n = rules.n_philo;

    i = 0;
    while (i < n)
    {
        if (pthread_create(&philos[i].thread, NULL, philos_routine, &philos[i]))
        {
            printf("Error: Failed to create thread\n");
            cleanup_resources(philos, forks, &rules, n);
            return (1);
        }
        i++;
    }

    if (pthread_create(&monitor, NULL, monitor_thread, philos))
    {
        printf("Error: Failed to create monitor thread\n");
        cleanup_resources(philos, forks, &rules, n);
        return (1);
    }

    i = 0;
    while (i < n)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    pthread_join(monitor, NULL);
    
    cleanup_resources(philos, forks, &rules, n);
    return (0);
}