/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_memento_mori.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/04 22:30:24 by febouana          #+#    #+#             */
/*   Updated: 2024/10/06 22:18:49 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

bool_t stop_signal(data_t *data, bool_t dead)
{
    pthread_mutex_lock(&data->m_stop);
    if (data->stop == true)
    {
        dead = true;
        pthread_mutex_unlock(&data->m_stop);
        return (true);
    }
    pthread_mutex_unlock(&data->m_stop);
    return (false);
}

void will_die(data_t *data, int id)
{
    pthread_mutex_lock(&data->m_stop);
    if (data->stop == false)
    {
        data->stop = true;
        unlock_forks(data, id);
        usleep(data->time_to_die * 1000);   
        print_all_action(data, 4, id, get_current_time() - data->start_time);
        data->philosophers[id].is_dead = true;
    }
    pthread_mutex_unlock(&data->m_stop);
}

int routine_solo(data_t *data, int id)
{
    pthread_mutex_lock(&data->philosophers[id].fork_l);
    data->philosophers[id].left_locked = true;
    print_all_action(data, 0, id, get_current_time() - data->start_time);
    will_die(data, id);
    return (0);
}
