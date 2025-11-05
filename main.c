/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:12:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/05 18:06:02 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

typedef struct s_data {
    int id;
    int n;
    pthread_mutex_t *forks;
} t_data;
long get_time_in_ms(void)
{
    struct timeval tv;
    long    ms;

    gettimeofday(&tv, NULL);
    ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) / 1000;
    return (ms);
}
void *philosofers(void *arg)
{
    t_data *data = (t_data *)arg;
    int id = data->id;
    int n = data->n;
    pthread_mutex_t *forks = data->forks;
    int left = id;
    int right = (id + 1) % n;
    int meals = 0;

    while(meals < 2)
    {
        printf("filosofo %d esta pensando\n", id);
        usleep(200000);
        pthread_mutex_lock(&forks[left]);
        pthread_mutex_lock(&forks[right]);
        printf("filosofo %d pegou os garfos e esta comendo 🍝\n", id);
        usleep(300000);
        pthread_mutex_unlock(&forks[left]);
        pthread_mutex_unlock(&forks[right]);
        printf("filoso %d terminou de comer\n", id);
        meals++;
    }
    return (NULL);
}
int main(int argc, char **argv)
{
    if (argc < 2)
        return (0);
    int n = atoi(argv[1]);
    pthread_mutex_t forks[n];
    pthread_t       threads[n];
    t_data  data[n];
    int     i;

    i = 0;
    while(i < n)
    {
        pthread_mutex_init(&forks[i], NULL);
        i++;
    }
    i = 0;
    while(i < n)
    {
        data[i].id = i;
        data[i].n = n;
        data[i].forks = forks;
        pthread_create(&threads[i], NULL, philosofers, &data[i]);
        i++;
    }
    i = 0;
    while(i < n)
    {
        pthread_join(threads[i], NULL);
        i++;
    }
    i = 0;
    while(i < n)
    {
        pthread_mutex_destroy(&forks[i]);
        i++;
    }
    printf("todos os filosofos terminaram.\n");
    return (0);
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