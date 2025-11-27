/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: guisanto <guisanto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/22 18:49:54 by guisanto          #+#    #+#             */
/*   Updated: 2025/11/27 18:16:05 by guisanto         ###   ########.fr       */
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
