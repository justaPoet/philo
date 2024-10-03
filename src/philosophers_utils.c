/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:30:39 by febouana          #+#    #+#             */
/*   Updated: 2024/10/03 21:24:28 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//? Permet de retourner le temps de depart EPOCH.
long long    get_current_time(void)
{
    struct timeval    tv;
    long long        milliseconds;

    if (gettimeofday(&tv, NULL) == -1)
        return (-1);
    milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
    return (milliseconds);
}

void destroy_fork(data_t *data)
{
    int i;
    
    i = 0;
    while (i < data->nbr_philos)
    {
        if (pthread_mutex_destroy(&data->philosophers[i].fork_l) != 0)
        {
            printf("BAMBPOCLAT destroy fork\n");
            return ; //!GESTION ERROR
        }
        i++;
    }   
}

// int  check_death(data_t *data, int id)
// { 
//     if (data->time_to_die <= (data->philosophers[id].last_meal - data->philosophers[id].last_last_meal))
//     {
//         if (data->dead == false)
//         {    
//             data->dead = true;
//             data->id_philo_dead = id;
//             data->time_death =  get_current_time() - data->start_time;
//             unlock_forks(data, id);
//         }
//         return (2);
//     }
//     return (0);
// }

//! check death solo
// int  check_death_solo(data_t *data, int id)
// {
//     if (data->philosophers[id].left_locked)
//         pthread_mutex_unlock(&data->philosophers[id].fork_l);
//     usleep(data->time_to_die * 1000);
//     data->philosophers[id].is_dead = true; //!
//     data->id_philo_dead = id;
//     data->time_death =  get_current_time() - data->start_time;
//     return (2);
// }

void unlock_forks(data_t *data, int id) 
{
    if (data->philosophers[id].right_locked == true)
    {
        pthread_mutex_unlock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = false;
    }
    if (data->philosophers[id].left_locked == true)
    {
        pthread_mutex_unlock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = false;
    }
}

int lock_first_fork(data_t *data, int id, bool_t dead)
{
    if (stop_signal(data) || dead)
    {
        unlock_forks(data, id);
        return (2);
    }
    if (id % 2 != 0)
    {
        pthread_mutex_lock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = true;
    }
    else 
    {
        pthread_mutex_lock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = true;
    }
    print_all_action(data, 0, id, get_current_time() - data->start_time, dead);
    return (0);
}

int lock_second_fork(data_t *data, int id, bool_t dead)
{
    if (stop_signal(data) || dead)
    {      
        unlock_forks(data, id);
        return (2);
    }
    if (id % 2 != 0)
    {
        pthread_mutex_lock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = true;
    }
    else
    {    
        pthread_mutex_lock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = true;
    }    
    print_all_action(data, 0, id, get_current_time() - data->start_time, dead); //!
    return (0);
}

int	print_all_action(data_t *data, int option, int id, long long time, bool_t dead)
{
    pthread_mutex_lock(&data->print);
    if (dead)
    {
        pthread_mutex_unlock(&data->print);
        return (2);
	}
    char	*actions[5] = {
		"%ld 🍴 (%d) has taken a fork\n",
		"%ld 🍝 (%d) is eating\n",
		"%ld 💤 (%d) is sleeping\n",
		"%ld 🤔 (%d) is thinking\n",
		"%ld ☠️  (%d) died\n"};
        
	printf(actions[option], time, id + 1);
    pthread_mutex_unlock(&data->print);
    return (0);
}
