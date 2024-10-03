/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:52:56 by febouana          #+#    #+#             */
/*   Updated: 2024/10/03 21:32:18 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

bool_t stop_signal(data_t *data)
{
    pthread_mutex_lock(&data->m_stop);
    if (data->stop == true)
    {
        pthread_mutex_unlock(&data->m_stop);
        return (true);
    }
    pthread_mutex_unlock(&data->m_stop);
    return (false);
}

int verif_sleeping(data_t *data, long time_to_sleep, int id, bool_t dead)
{
    if (stop_signal(data) || dead)
        return (2);        
    if (time_to_sleep >= data->time_to_die)
    {
        pthread_mutex_lock(&data->m_stop);
        if (data->stop == false)
        {
            data->stop = true;
            data->philosophers[id].is_dead = true;
            unlock_forks(data, id);
            usleep(data->time_to_die * 1000);
                        
            print_all_action(data, 4, id, get_current_time() - data->start_time, dead); //!
            dead = true;
        }
        pthread_mutex_unlock(&data->m_stop);
        return (2);
    }
    usleep(time_to_sleep * 1000);
    print_all_action(data, 2, id, get_current_time() - data->start_time, dead); //!
    return (0);
}


int verif_eating(data_t *data, long time_to_eat, int id, bool_t dead)
{    
    if (stop_signal(data) || dead)
        return (2);
        
    lock_first_fork(data, id, dead);
    lock_second_fork(data, id, dead);
    
    //if (data->time_to_eat >= data->time_to_die)

    //printf("last_meal - last_last_meal==%lld\n", data->philosophers[id].last_meal - data->philosophers[id].last_last_meal);
    
    if (data->time_to_die <= (data->philosophers[id].last_meal - data->philosophers[id].last_last_meal))
    {
        pthread_mutex_lock(&data->m_stop);
        if (data->stop == false)
        {
            data->stop = true;
            data->philosophers[id].is_dead = true;
            unlock_forks(data, id);
            usleep(data->time_to_die * 1000);
                        
            print_all_action(data, 4, id, get_current_time() - data->start_time, dead); //!
            dead = true;
        }
        pthread_mutex_unlock(&data->m_stop);
        return (2);
    }
    // printf("last_last_meal==%lld | last_meal==%lld\n", data->philosophers[id].last_last_meal, data->philosophers[id].last_meal);
    
    data->philosophers[id].last_last_meal = data->philosophers[id].last_meal; //?safe
    data->philosophers[id].last_meal =  get_current_time() - data->start_time;  //?safe
    print_all_action(data, 1, id, get_current_time() - data->start_time, dead); //!
    //printf(">>last_meal - last_last_meal==%lld\n", data->philosophers[id].last_meal - data->philosophers[id].last_last_meal);
    usleep(time_to_eat * 1000);
    unlock_forks(data, id);
    return (0);
}

int complet_routine(data_t *data, int id)
{
    bool_t dead;
    dead = false;

    // if (data->nbr_philos == 1)
    // {
    //     routine_solo();
    //     return (2);
    // }
    
    if (id % 2 != 0) //! verif premier sleep si < a time to die
        usleep(data->time_to_eat * 1000);
    while(data->philosophers[id].repeat_meal_philo != 0)
    { 
        if (data->philosophers[id].is_dead == true)
            break ;
        if (verif_eating(data, data->time_to_eat, id, data->philosophers[id].is_dead) == 2)
            break ;
        if (verif_sleeping(data, data->time_to_sleep, id, data->philosophers[id].is_dead) == 2) //?safe
            break ;
        print_all_action(data, 3, id, get_current_time() - data->start_time, data->philosophers[id].is_dead); //? safe
        data->philosophers[id].repeat_meal_philo--; //?safe
    }
    unlock_forks(data, id);
    return (0);
}

void	*philosopher_routine(void *philo)
{
    data_idx_t data_index = *(data_idx_t *)philo;
    free(philo);
    
    //?rajouter condition si time_to_eat >= time_to_die ??? // verif_routine ???????

    
    complet_routine(data_index.data, data_index.idx);
    
    return (NULL);
}


//!1/ reimplanter le systeme de mort 
//! -meurt de faim 
//! -meurt car diff de temps pour dormir 
//! -meurt car diff de temps pour manger)
//!2/ reimplanter cas avec 1 seul philo
























// bool_t philo_is_dead(data_t *data)
// {
//     int i;
    
//     i = 0;
//     pthread_mutex_lock(&data->will_die);
//     while (i < data->nbr_philos)
//     {
//         if (data->philosophers[i].is_dead == true)
//         {
//             pthread_mutex_unlock(&data->will_die);
//             return (true);
//         }
//         i++;
//     }             
//     pthread_mutex_unlock(&data->will_die);
//     return (false);
// }


// int verif_eating(data_t *data, long time_to_eat, int id, bool_t dead)
// {    
//     if (data->stop || dead)
//         return (2);        
//     lock_first_fork(data, id, dead);
//     lock_second_fork(data, id, dead);
//     if (time_to_eat >= data->time_to_die)
//     {   
//         pthread_mutex_lock(&data->will_die); //raf?
//         if (data->stop == false)
//         {
//             data->stop = true;
//             dead = true;
//             data->id_philo_dead = id;
//             data->time_death =  get_current_time() - data->start_time;
//             unlock_forks(data, id);
//         }
//         pthread_mutex_unlock(&data->will_die); //raf?
//         unlock_forks(data, id);
//         usleep(data->time_to_die * 1000);
//         return (2);
//     }
//     data->philosophers[id].last_last_meal = data->philosophers[id].last_meal; //?safe
//     data->philosophers[id].last_meal =  get_current_time() - data->start_time;  //?safe
//     print_all_action(data, 1, id, get_current_time() - data->start_time, dead); //!
//     usleep(time_to_eat * 1000);
//     unlock_forks(data, id);
//     return (0);
// }