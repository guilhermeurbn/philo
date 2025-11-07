/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:12:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/07 19:47:25 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#define TIME_TO_DIE 600

//a funcao pthread_join ela faz com oq uma thread(fio) 
//espere pelo o outro termine como se fosse um wait()

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
    pthread_mutex_t *left_fork;
    pthread_mutex_t *right_fork;
    t_rules *rules;
} t_philo;



long get_time_in_ms(void)
{
    struct timeval tv;
    long    ms;

    gettimeofday(&tv, NULL);
    ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000);
    return (ms);
}

void *check_death(void *arg)
{
    t_data *data = (t_data *)arg;
    long now;
    while (!data->finished)
    {
        pthread_mutex_lock(&death_mutex);
        if (someone_died)
        {
            pthread_mutex_unlock(&death_mutex);
            break;
        }
        pthread_mutex_unlock(&death_mutex);
        now = get_time_in_ms();
        if ((now - data->last_meal) > TIME_TO_DIE)
        {
            pthread_mutex_lock(&death_mutex);
            if (!someone_died)
            {
                someone_died = 1;
                printf("Filoso %d morreu\n", data->id);
                printf("se passaram %ld ms desda sua last_meal\n", now - data->last_meal);
            }
            pthread_mutex_unlock(&death_mutex);
            break;
        }
        usleep(1000);
    }
    return (NULL);
}

void *philosofers(void *arg)
{
    t_data *data = (t_data *)arg;
    pthread_mutex_t *forks = data->forks;
    int id = data->id;
    int n = data->n;
    int left = id;
    int right = (id + 1) % n;
    int meals = 0;

    while(meals < 2)
    {
        pthread_mutex_lock(&death_mutex);
        if (someone_died)
        {
            pthread_mutex_unlock(&death_mutex);
            return (NULL);
        }
        pthread_mutex_unlock(&death_mutex);

        printf("filosofo %d esta pensando\n", id);
        usleep(200000);
        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);
        data->last_meal = get_time_in_ms();
        printf("filosofo %d pegou os garfos e esta comendo 🍝\n", id);
        usleep(300000);
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
        printf("filosofo %d terminou de comer\n", id);
        meals++;
    }
    data->finished = 1;
    return (NULL);
}
int init_rules(t_rules *rules, int argc, char **argv)
{
    if (argc != 5)
    {
		printf("Ex: ./philo n_philo time_to_die time_to_eat time_to_sleep\n");
		return (1);
	}
    rules->n_philo = atoi(argv[1]);
    rules->time_to_die = atol(argv[2]);
    rules->time_to_eat = atol(argv[3]);
    rules->time_to_sleep = atol(argv[4]);
    rules->someone_died = 0;
    rules->start_time = get_time_in_ms();
    pthread_mutex_init(&rules->death_mutex, NULL);
    pthread_mutex_init(&rules->print_mutex, NULL);
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
        pthread_mutex_init(&forks[i], NULL);
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

    while(1)
    {
        pthread_mutex_lock(&rules->death_mutex);
        if (rules->someone_died)
            return (pthread_mutex_unlock(&rules->death_mutex), NULL);
        pthread_mutex_unlock(&rules->death_mutex);
        
        pthread_mutex_lock(p->left_fork);
        pthread_mutex_lock(p->right_fork);
        
        p->last_meal = get_time_in_ms();
        pthread_mutex_lock(&rules->print_mutex);
        printf("%ld ms: Filosofo %d esta comendo", (get_time_in_ms() - rules->start_time), p->id);
        pthread_mutex_unlock(&rules->print_mutex);
        usleep(rules->time_to_eat * 1000);

        
        
    }
}
int main(int argc, char **argv)
{
    t_rules *rules;
    t_philo philos[200]; //limite so um exemplo
    pthread_mutex_t forks[200];
    
    if (init_rules(&rules, argc, argv))
        return (1);
    init_philos(philos, forks, &rules);
    int i;
    int n = rules->n_philo;

    /* criar threads */
    i = 0;
    while (i < n)
    {
        pthread_create(&philos[i].thread, NULL, philos_routine, &philos[i]);
        usleep(1000);
        i++;
    }
    i = 0;
    while(i < n)
    {
        pthread_join(philos[i].thread, NULL);
        i++;
    }
}

/*
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void    *thread_function(void *arg);
int main(int ac, char **av)
{
    if (ac != 2)
        return 0;

    int n = atoi(av[1]);
    pthread_t thread[2];
    int ids[n];

    int i = 0;
    long start, end;

    start = get_time_in_ms();
    while (i < n)
    {
        ids[i] = i + 1;
        usleep(5000);
        pthread_create(&thread[i], NULL, thread_function, NULL);
        i++;
    }

    i = 0;
    while (i < n)
    {
        pthread_join(thread[i], NULL);
        i++;
    }
    end = get_time_in_ms() - start;
    printf("Todas %d threads finalizadas em %ld ms\n", n, end);
    return 0;
}
void *thread_function(void *arg)
{
    (void)arg;
    int count = 0;

    while (count < 10)
    {
        pthread_mutex_lock(&mutex);
        printf("Thread %ld count = %d\n", (unsigned long)pthread_self(), count);
        count++;
        pthread_mutex_unlock(&mutex);   
    }
    pthread_exit(NULL);
} */