/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:52:56 by febouana          #+#    #+#             */
/*   Updated: 2024/10/05 17:24:21 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int verif_sleeping_thinking(data_t *data, long time_to_sleep, int id)
{    
    if (time_to_sleep >= data->time_to_die)
    {
        will_die(data, id);
        return (2);
    }
    if (stop_signal(data, data->philosophers[id].is_dead))
        return (2);
    usleep(time_to_sleep * 1000);
    print_all_action(data, 2, id, get_current_time() - data->start_time); //!
    print_all_action(data, 3, id, get_current_time() - data->start_time); //? safe
    return (0);
}

int verif_eating(data_t *data, long time_to_eat, int id)
{    
    // printf("$$$$$$$<%d> last_meal - last_last_meal==%lld\n", id + 1, (get_current_time() - data->start_time) - data->philosophers[id].last_meal);
    if (data->time_to_die < ((get_current_time() - data->start_time) - data->philosophers[id].last_meal))
    {
        will_die(data, id);
        return (2);
    }
    if (stop_signal(data, data->philosophers[id].is_dead))
        return (2);
    data->philosophers[id].last_meal =  get_current_time() - data->start_time;  //?safe
    usleep(time_to_eat * 1000);
    print_all_action(data, 1, id, get_current_time() - data->start_time); //!
    unlock_forks(data, id);
    return (0);
}

int lock_second_fork_odd(data_t *data, int id, bool_t dead)
{
    if (dead)
        return (2);   
    if (id == 0)
    {
        pthread_mutex_lock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = true;
    }
    else
    {    
        pthread_mutex_lock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = true;
    }    
    print_all_action(data, 0, id, get_current_time() - data->start_time);
    return (0);
}

int lock_first_fork_odd(data_t *data, int id, bool_t dead)
{
    if (dead)
        return (2); 
    if (id + 1 == data->nbr_philos)
    {
        pthread_mutex_lock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = true;
    }
    else 
    {
        pthread_mutex_lock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = true;
    }
    print_all_action(data, 0, id, get_current_time() - data->start_time);
    return (0);
}

int take_forks(data_t *data, int id, bool_t dead)
{
    if (id == 0 || id + 1 == data->nbr_philos) //?OKOK
    {
        lock_first_fork_odd(data, id, dead);
        lock_second_fork_odd(data, id, dead);
    }       
    else 
    {
        lock_first_fork(data, id, dead);
        lock_second_fork(data, id, dead);
    }
    return (0);
}

//./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] ([number_of_times_each_philosopher_must_eat])

int complet_routine(data_t *data, int id)
{
    if (data->time_to_die <= (data->time_to_eat))
    {
        will_die(data, id);
        return (2);
    }
    else 
    {
        if (id % 2 != 0)
            usleep(data->time_to_eat * 1000);         
    }
    while(data->philosophers[id].repeat_meal_philo != 0)
    {   
        if (data->philosophers[id].is_dead || stop_signal(data, data->philosophers[id].is_dead))
            break ;
        take_forks(data, id, data->philosophers[id].is_dead);
        verif_eating(data, data->time_to_eat, id);
        verif_sleeping_thinking(data, data->time_to_sleep, id);
        data->philosophers[id].repeat_meal_philo--; //?safe
    }
    unlock_forks(data, id);
    return (0);
}

void	*philosopher_routine(void *philo)
{
    data_idx_t data_index = *(data_idx_t *)philo;
    free(philo);
    
    if (data_index.data->nbr_philos == 1)
        routine_solo(data_index.data, data_index.idx);
    else
        complet_routine(data_index.data, data_index.idx);
    return (NULL);
}

















