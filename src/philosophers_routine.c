/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:52:56 by febouana          #+#    #+#             */
/*   Updated: 2024/10/07 00:29:19 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	unlock_forks(data_t *data, int id)
{
	if (data->philosophers[id].id % 2 == 0)
	{
		if (data->philosophers[id].left_locked == true)
			pthread_mutex_unlock(&data->philosophers[id].fork_l);
		if (data->philosophers[id].right_locked == true)
			pthread_mutex_unlock(data->philosophers[id].fork_r);
        data->philosophers[id].left_locked = false;
		data->philosophers[id].right_locked = false;
	}
	else
	{
		if (data->philosophers[id].right_locked == true)
			pthread_mutex_unlock(data->philosophers[id].fork_r);
		if (data->philosophers[id].left_locked == true)
			pthread_mutex_unlock(&data->philosophers[id].fork_l);
        data->philosophers[id].right_locked = false;
        data->philosophers[id].left_locked = false;
	}
}

int	lock_forks(data_t *data, int id, bool_t dead)
{
	if (stop_signal(data, dead))
		return (2);
	if (data->philosophers[id].id % 2 == 0)
	{
		pthread_mutex_lock(&data->philosophers[id].fork_l);
		data->philosophers[id].left_locked = true;
		print_all_action(data, 0, id, get_current_time() - data->start_time);
		pthread_mutex_lock(data->philosophers[id].fork_r);
		data->philosophers[id].right_locked = true;
		print_all_action(data, 0, id, get_current_time() - data->start_time);
	}
	else
	{
		pthread_mutex_lock(data->philosophers[id].fork_r);
		data->philosophers[id].right_locked = true;
		print_all_action(data, 0, id, get_current_time() - data->start_time);
		pthread_mutex_lock(&data->philosophers[id].fork_l);
		data->philosophers[id].left_locked = true;
		print_all_action(data, 0, id, get_current_time() - data->start_time);
	}
	return (0);
}

long long int	get_time_in_ms(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
	{
		printf("Error: Cannot Get Time\n");
		return (0);
	}
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

//+10 => 2 410 200 200
//+500? 1000? 10000 == moins d'erreur pour les plus gros test
void	ft_usleep(long long int time)
{
	long long int	start_time;

	start_time = get_time_in_ms();
	while (get_time_in_ms() - start_time < time)
		usleep(10000);
}

int	verif_sleeping_thinking(data_t *data, long time_to_sleep, int id)
{
	if (time_to_sleep >= data->time_to_die)
	{
		will_die(data, id);
		return (2);
	}
	if (stop_signal(data, data->philosophers[id].is_dead))
		return (2);
	print_all_action(data, 2, id, get_current_time() - data->start_time);
	usleep(time_to_sleep * 1000);
	print_all_action(data, 3, id, get_current_time() - data->start_time);
	ft_usleep((data->time_to_eat > data->time_to_sleep) * (data->time_to_eat
			- data->time_to_sleep) + 1);
	return (0);
}

int	verif_eating(data_t *data, long time_to_eat, int id)
{
	if (data->time_to_die < ((get_current_time() - data->start_time)
			- data->philosophers[id].last_meal))
	{
		will_die(data, id);
		return (2);
	}
	if (stop_signal(data, data->philosophers[id].is_dead))
		return (2);
	data->philosophers[id].last_meal = get_current_time() - data->start_time;
	print_all_action(data, 1, id, get_current_time() - data->start_time);
	usleep(time_to_eat * 1000);
	unlock_forks(data, id);
	return (0);
}

//./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] ([number_of_times_each_philosopher_must_eat])

int	complet_routine(data_t *data, int id)
{
	if (data->time_to_die <= (data->time_to_eat))
	{
		will_die(data, id);
		return (2);
	}
	if (data->philosophers[id].id % 2 != 0)
		usleep(data->time_to_eat * 1000);
	while (data->philosophers[id].repeat_meal_philo != 0)
	{
		if (data->philosophers[id].is_dead || stop_signal(data,
				data->philosophers[id].is_dead))
			break ;
		lock_forks(data, id, data->philosophers[id].is_dead);
		verif_eating(data, data->time_to_eat, id);
		verif_sleeping_thinking(data, data->time_to_sleep, id);
		data->philosophers[id].repeat_meal_philo--; //?safe
	}
	unlock_forks(data, id);
	return (0);
}

void	*philosopher_routine(void *philo)
{
	data_idx_t	data_index;

	data_index = *(data_idx_t *)philo;
	free(philo);
	if (data_index.data->nbr_philos == 1)
		routine_solo(data_index.data, data_index.idx);
	else
		complet_routine(data_index.data, data_index.idx);
	return (NULL);
}

// int lock_forks(data_t *data, int id, bool_t dead)
// {

//     if (stop_signal(data, dead))
//         return (2);
//     if (data->philosophers[id].right_locked == false)
//     {
//         pthread_mutex_lock(data->philosophers[id].fork_r);
//         data->philosophers[id].right_locked = true;
//         print_all_action(data, 0, id, get_current_time() - data->start_time);
//     }
//     if (data->philosophers[id].left_locked == false)
//     {
//         pthread_mutex_lock(&data->philosophers[id].fork_l);
//         data->philosophers[id].left_locked = true;
//         print_all_action(data, 0, id, get_current_time() - data->start_time);
//     }
//     return (0);
// }

// void unlock_forks(data_t *data, int id)
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
