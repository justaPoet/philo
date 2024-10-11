/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_forks.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 17:39:05 by febouana          #+#    #+#             */
/*   Updated: 2024/10/11 22:33:37 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	create_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_philos)
	{
		if (pthread_mutex_init(&data->philosophers[i].fork_l, NULL) != 0)
		{
			error_quit(data, i);
			return (2);
		}
		i++;
	}
	assign_fork(data);
	return (0);
}

int	lock_forks(t_data *data, int id)
{
	if (stop_signal(data, data->philosophers[id].is_dead))
		return (2);
	if (data->philosophers[id].id % 2 == 0)
	{
		pthread_mutex_lock(&data->philosophers[id].fork_l);
		data->philosophers[id].left_locked = true;
		ft_print(data, 0, id, get_current_time() - data->start_time);
		pthread_mutex_lock(data->philosophers[id].fork_r);
		data->philosophers[id].right_locked = true;
		ft_print(data, 0, id, get_current_time() - data->start_time);
	}
	else
	{
		pthread_mutex_lock(data->philosophers[id].fork_r);
		data->philosophers[id].right_locked = true;
		ft_print(data, 0, id, get_current_time() - data->start_time);
		pthread_mutex_lock(&data->philosophers[id].fork_l);
		data->philosophers[id].left_locked = true;
		ft_print(data, 0, id, get_current_time() - data->start_time);
	}
	return (0);
}

void	direction_unlock_forks(t_data *data, int id)
{
	if (data->philosophers[id].id % 2 == 0)
		unlock_forks(data, id);
	else
		unlock_forks_odd(data, id);
}

void	unlock_forks(t_data *data, int id)
{
	if (data->philosophers[id].left_locked == true)
	{
		pthread_mutex_unlock(&data->philosophers[id].fork_l);
		data->philosophers[id].left_locked = false;
	}
	if (data->philosophers[id].right_locked == true)
	{
		pthread_mutex_unlock(data->philosophers[id].fork_r);
		data->philosophers[id].right_locked = false;
	}
}

void	unlock_forks_odd(t_data *data, int id)
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
