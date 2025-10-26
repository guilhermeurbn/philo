/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:12:36 by guisanto          #+#    #+#             */
/*   Updated: 2025/10/26 19:01:53 by guisanto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <unistd.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


void    *thread_function(void *arg);
int main(int ac, char **av)
{
    if (ac != 2)
        return 0;

    int n = atoi(av[1]);
    pthread_t thread[n];
    int ids[n];

    int i = 0;
    while (i < n)
    {
        ids[i] = i + 1;
        pthread_create(&thread[i], NULL, thread_function, NULL);
        i++;
    }

    i = 0;
    while (i < n)
    {
        pthread_join(thread[i], NULL);
        i++;
    }

    printf("Todas %d threads finalizadas\n", n);
    return 0;
}
void *thread_function(void *arg)
{
    (void)arg;
    int count = 0;

    while (count < 3) // cada thread vai tentar acessar 3 vezes
    {
        pthread_mutex_lock(&mutex); // início da seção crítica
        printf("Thread %ld count = %d\n", (unsigned long)pthread_self(), count);
        count++;
        pthread_mutex_unlock(&mutex); // fim da seção crítica
    }
    pthread_exit(NULL);
}
