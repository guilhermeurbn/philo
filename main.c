/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/12 12:32:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/20 17:54:14 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#define TIME_TO_DIEd 600

//a funcao pthread_join ela faz com oq uma thread(fio) 
//espere pelo o outro termine como se fosse um wait()

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
        usleep(500);
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
        print_action(p->rules, p->id, "has taken a fork");
        pthread_mutex_lock(p->left_fork);
        print_action(p->rules, p->id, "has taken a fork");

    }
    else
    {
        pthread_mutex_lock(p->left_fork);
        print_action(p->rules, p->id, "has taken a fork");
        pthread_mutex_lock(p->right_fork);
        print_action(p->rules, p->id, "has taken a fork");
    }
}
void drop_forks(t_philo *p)
{
    pthread_mutex_unlock(p->left_fork);
    pthread_mutex_unlock(p->right_fork);
    
}
void *monitor_thread(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_rules *rules = philos[0].rules;

    long now;
    int i;
    int all_ate;

    while (1)
    {
        i = 0;
        all_ate = 1;
        while (i < rules->n_philo)
        {
            pthread_mutex_lock(&philos[i].meal_mutex);
            now = get_time_in_ms();

            if ((now - philos[i].last_meal) > rules->time_to_died)
            {
                pthread_mutex_unlock(&philos[i].meal_mutex);

                pthread_mutex_lock(&rules->death_mutex);
                if (!rules->someone_died)
                {
                    rules->someone_died = 1;
                    pthread_mutex_unlock(&rules->death_mutex);
                    print_action(rules, philos[i].id, "Died");
                    return NULL;
                }
                pthread_mutex_unlock(&rules->death_mutex);
                return NULL;
            }

            if (rules->must_eat > 0 && philos[i].meals_eaten < rules->must_eat)
                all_ate = 0;

            pthread_mutex_unlock(&philos[i].meal_mutex);
            i++;
        }

        if (rules->must_eat > 0 && all_ate)
        {
            pthread_mutex_lock(&rules->death_mutex);
            rules->someone_died = 1;
            pthread_mutex_unlock(&rules->death_mutex);
            return NULL;
        }
        usleep(1000);
    }
}

int init_rules(t_rules *rules, int argc, char **argv)
{
    if (argc != 5 && argc != 6)
    {
		printf("Ex: ./philo n_philo time_to_died time_to_eat time_to_sleep [must_eat]\n");
		exit(0);
	}
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
    pthread_mutex_init(&rules->death_mutex, NULL);
    pthread_mutex_init(&rules->print_mutex, NULL);
    return (0);
}
int init_philos(t_philo *philos, pthread_mutex_t *forks, t_rules *rules)
{
    if (rules->n_philo <= 0 || rules->n_philo > 200 || rules->time_to_died <= 0 || rules->time_to_eat <= 0 
        || rules->time_to_sleep <= 0)
    {
        printf("Error: Fora dos parametros\n");
        exit(0);
    }
    int i;
    int n;
    
    i = 0;
    n = rules->n_philo;

    while(i < n)
    {
        pthread_mutex_init(&forks[i], NULL);
        pthread_mutex_init(&philos[i].meal_mutex, NULL);
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
void *philos_routine(void *arg)
{
    t_philo *p;
    t_rules *rules;

    p = (t_philo *)arg;
    rules = p->rules;

    //condicao para apenas 1 philo
    if (rules->n_philo == 1)
    {
        pthread_mutex_lock(p->left_fork);
        print_action(rules, p->id, "has taken a fork");
        //vamos esperar o time_to_died acabar
        smart_sleep(rules->time_to_died, rules);
        pthread_mutex_unlock(p->left_fork);
        
        set_dead(rules);
        print_action(rules, p->id, "Died");
        printf("se passaram %ld ms\n",  get_time_in_ms () - rules->start_time);
        return NULL;
    }
    while (!is_dead(rules))
    {
        if (is_dead(rules))
            break;
        take_forks(p);
        
        pthread_mutex_lock(&p->meal_mutex);
        p->last_meal = get_time_in_ms();
        p->meals_eaten++;
        pthread_mutex_unlock(&p->meal_mutex);
        
        print_action(rules, p->id, "Eating");
        smart_sleep(rules->time_to_eat, rules);
        
        if (is_dead(rules))
            break;
        drop_forks(p);
        
        if (is_dead(rules))
            break;
        print_action(rules, p->id, "Sleeping");
        smart_sleep(rules->time_to_sleep, rules);
            
        if (is_dead(rules)) 
            break;
        print_action(rules, p->id, "Thinking");            
    }
    return (NULL);
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
    init_philos(philos, forks, &rules);
    n = rules.n_philo;

    i = 0;
    while (i < n)
    {
        pthread_create(&philos[i].thread, NULL, philos_routine, &philos[i]);
        usleep(1000);
        i++;
    }

    pthread_create(&monitor, NULL, monitor_thread, philos);

    i = 0;
    while (i < n)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
    pthread_join(monitor, NULL);
    i = 0;
    while (i < n)
    {
        pthread_mutex_destroy(&forks[i]);
        pthread_mutex_destroy(&philos[i].meal_mutex);
        i++;
    }
    pthread_mutex_destroy(&rules.death_mutex);
    pthread_mutex_destroy(&rules.print_mutex);
    return (0);
}