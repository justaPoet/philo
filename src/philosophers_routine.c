/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:52:56 by febouana          #+#    #+#             */
/*   Updated: 2024/10/08 17:46:56 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//? + > - == 1 | + < - == 0
int	verif_thinking(data_t *data, int id)
{
	if (stop_signal(data, data->philosophers[id].is_dead))
		return (2);
	ft_print(data, 3, id, get_current_time() - data->start_time);
	usleep((data->time_to_eat > data->time_to_sleep) * (data->time_to_eat
			- data->time_to_sleep) + 1 * 1000);
	return (0);
}

int	verif_sleeping(data_t *data, long time_to_sleep, int id)
{
	if (time_to_sleep >= data->time_to_die)
	{
		will_die(data, id);
		return (2);
	}
	if (stop_signal(data, data->philosophers[id].is_dead))
		return (2);
	ft_print(data, 2, id, get_current_time() - data->start_time);
	usleep(time_to_sleep * 1000);
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
	ft_print(data, 1, id, get_current_time() - data->start_time);
	usleep(time_to_eat * 1000);
	direction_unlock_forks(data, id);
	return (0);
}

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
		lock_forks(data, id);
		verif_eating(data, data->time_to_eat, id);
		verif_sleeping(data, data->time_to_sleep, id);
		verif_thinking(data, id);
		data->philosophers[id].repeat_meal_philo--; //?safe
	}
	direction_unlock_forks(data, id);
	return (0);
}

void	*philosopher_routine(void *philo)
{
	data_tmp_t	data_tmp;

	data_tmp = *(data_tmp_t *)philo;
	free(philo);
	if (data_tmp.data->nbr_philos == 1)
		routine_solo(data_tmp.data, data_tmp.id);
	else
		complet_routine(data_tmp.data, data_tmp.id);
	return (NULL);
}
