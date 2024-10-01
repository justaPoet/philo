/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:52:56 by febouana          #+#    #+#             */
/*   Updated: 2024/10/01 17:18:28 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

// int verif_routine(data_t *data, int id)
// {
//     if (data->time_to_eat >= data->time_to_die)
//     {   
//         if (data->dead == false)
//         {    
//             data->dead = true;
//             data->id_philo_dead = id;
//             data->time_death =  get_current_time() - data->start_time;
//             unlock_forks(data, id);
//         }
//         usleep(data->time_to_die * 1000);
//         return (2);
//     }       
//     return (0);
// }

int lock_first_fork(data_t *data, int id, bool_t status)
{
    if (status == true)
    {
        unlock_forks(data, id);
        return (2);
    }
    print_all_action(data, 0, id, get_current_time() - data->start_time, status);
    if (id & 1)
    {
        pthread_mutex_lock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = true;
    }
    else 
    {
        pthread_mutex_lock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = true;
    }
    return (0);
}

int lock_second_fork(data_t *data, int id, bool_t status)
{
    if (status == true)
    {
        unlock_forks(data, id);
        return (2);
    }
    print_all_action(data, 0, id, get_current_time() - data->start_time, status); //!
    if (id & 1)
    {
        pthread_mutex_lock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = true;
    }
    else
    {    
        pthread_mutex_lock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = true;
    }    
    return (0);
}

int verif_sleeping(data_t *data, long time_to_sleep, int id, bool_t status)
{
    if (status == true)
        return (2);
    // if (time_to_sleep >= data->time_to_die)
    // {   
    //     //pthread_mutex_lock(&data->will_die); //raf?
    //     data->stop = true;
    //     status = true;
    //     data->id_philo_dead = id;
    //     data->time_death =  get_current_time() - data->start_time;
    //     unlock_forks(data, id);
    //     //pthread_mutex_unlock(&data->will_die); //raf?
    //     usleep(data->time_to_die * 1000);
    //     return (2);
    // }
    print_all_action(data, 2, id, get_current_time() - data->start_time, status); //!
    usleep(time_to_sleep * 1000);
    return (0);
}

int verif_eating(data_t *data, long time_to_eat, int id, bool_t status)
{    
    if (status == true)
        return (2);        
    // if (time_to_eat >= data->time_to_die)
    // {
    //     pthread_mutex_lock(&data->will_die); //raf?
    //     status = true;
    //     data->id_philo_dead = id;
    //     data->time_death =  get_current_time() - data->start_time;
    //     unlock_forks(data, id);
    //     pthread_mutex_unlock(&data->will_die); //raf?
    //     usleep(data->time_to_die * 1000);
    //     return (2);
    // }
    data->philosophers[id].last_last_meal = data->philosophers[id].last_meal; //?safe
    data->philosophers[id].last_meal =  get_current_time() - data->start_time;  //?safe
    print_all_action(data, 1, id, get_current_time() - data->start_time, status); //!
    usleep(time_to_eat * 1000);
    return (0);
}

//! rajouter des break de partout 
//! C'EST A CAUSE DE L'ORDRE DE MERDE
int complet_routine(data_t *data, int id)
{
    bool_t status;
    status = false;
    
    //print_all_action(data, 3, id, get_current_time() - data->start_time, status); //? safe
    
    //?IF 1 philo
    if (id & 1)
        usleep(data->time_to_eat * 1000);
    while(data->stop == true || data->philosophers[id].repeat_meal_philo != 0)
    { 
        if (data->philosophers[id].is_dead == true)
            break ;
        lock_first_fork(data, id, status);
        lock_second_fork(data, id, status);
        
        if (verif_eating(data, data->time_to_eat, id, data->philosophers[id].is_dead) == 2)
        {
            unlock_forks(data, id);
            break ;
        }
        unlock_forks(data, id);
        if (verif_sleeping(data, data->time_to_sleep, id, data->philosophers[id].is_dead) == 2) //?safe
            break ;
        print_all_action(data, 3, id, get_current_time() - data->start_time, data->philosophers[id].is_dead); //? safe
        data->philosophers[id].repeat_meal_philo--; //?safe
    }
    return (0);
}

void	*philosopher_routine(void *index)
{
    data_t *data;
    int id;
    data = get_data();
    id = *(int*)index - 1;

    //?rajouter condition si time_to_eat >= time_to_die ??? // verif_routine ???????
    complet_routine(data, id);
    return (NULL);
}
