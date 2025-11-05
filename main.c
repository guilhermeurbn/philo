/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:12:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/05 16:19:28 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <sys/time.h>
#include <unistd.h>


long get_time_in_ms(void)
{
    struct timeval tv;
    long    ms;

    gettimeofday(&tv, NULL);
    ms = (tv.tv_sec * 1000) + (tv.tv_usec / 1000) / 1000;
    return (ms);
}

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
}


 