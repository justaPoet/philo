/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:30:39 by febouana          #+#    #+#             */
/*   Updated: 2024/10/05 17:27:35 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

long long    get_current_time(void)
{
    struct timeval    tv;
    long long        milliseconds;

    if (gettimeofday(&tv, NULL) == -1)
        return (-1);
    milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
    return (milliseconds);
}

void unlock_forks_odd(data_t *data, int id) 
{
    if (id == 0 && data->philosophers[id].right_locked && data->philosophers[id].right_locked)
    {
        pthread_mutex_unlock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = false;
        pthread_mutex_unlock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = false;
    }
    else if (id + 1 == data->nbr_philos && data->philosophers[id].right_locked && data->philosophers[id].right_locked)
    {
        pthread_mutex_unlock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = false;
        pthread_mutex_unlock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = false;
    }
}

void unlock_forks(data_t *data, int id) 
{
    if (id == 0 || id + 1 == data->nbr_philos) // si impair et first ou last
    {
        unlock_forks_odd(data, id);
        return ;
    }
    if (id % 2 != 0 && data->philosophers[id].right_locked && data->philosophers[id].left_locked) // impair
    {
        pthread_mutex_unlock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = false;
        pthread_mutex_unlock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = false;
    }
    else if (id % 2 == 0 && data->philosophers[id].left_locked && data->philosophers[id].right_locked) // pair
    {
        pthread_mutex_unlock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = false;
        pthread_mutex_unlock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = false;
    }
}

int lock_second_fork(data_t *data, int id, bool_t dead)
{
    if (dead)
        return (2);   
    if (id % 2 != 0 && data->philosophers[id].left_locked == false) // impair
    {
        pthread_mutex_lock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = true;
    }
    else if (id % 2 == 0 && data->philosophers[id].right_locked == false)// pair
    {   
        pthread_mutex_lock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = true;
    }    
    print_all_action(data, 0, id, get_current_time() - data->start_time);
    return (0);
}

int lock_first_fork(data_t *data, int id, bool_t dead)
{
    if (dead)
        return (2); 
    if (id % 2 != 0 && data->philosophers[id].right_locked == false) // impair
    {
        pthread_mutex_lock(data->philosophers[id].fork_r);
        data->philosophers[id].right_locked = true;
    }
    else if (id % 2 == 0 && data->philosophers[id].left_locked == false) // pair
    {
        pthread_mutex_lock(&data->philosophers[id].fork_l);
        data->philosophers[id].left_locked = true;
    }
    print_all_action(data, 0, id, get_current_time() - data->start_time);
    return (0);
}

int	print_all_action(data_t *data, int option, int id, long long time)
{
    pthread_mutex_lock(&data->m_print);
    if (data->philosophers[id].is_dead)
    {
        pthread_mutex_unlock(&data->m_print);
        return (2);
	}
    char	*actions[5] = {
		"%ld 🍴 (%d) has taken a fork\n",
		"%ld 🍝 (%d) is eating\n",
		"%ld 💤 (%d) is sleeping\n",
		"%ld 🤔 (%d) is thinking\n",
		"%ld ☠️  (%d) died\n"};
        
	printf(actions[option], time, id + 1);
    pthread_mutex_unlock(&data->m_print);
    return (0);
}

// void eventually_unlock_forks(data_t *data, int id) 
// {
//     if (data->philosophers[id].right_locked == true)
//     {
//         pthread_mutex_unlock(data->philosophers[id].fork_r);
//         data->philosophers[id].right_locked = false;
//     }
//     if (data->philosophers[id].left_locked == true)
//     {
//         pthread_mutex_unlock(&data->philosophers[id].fork_l);
//         data->philosophers[id].left_locked = false;
//     }
// }
