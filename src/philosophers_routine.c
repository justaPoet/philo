/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_routine.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/17 14:52:56 by febouana          #+#    #+#             */
/*   Updated: 2024/10/11 22:31:24 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

//? + > - == 1 | + < - == 0
int	verif_thinking(t_data *data, int id)
{
	if (stop_signal(data, data->philosophers[id].is_dead))
		return (2);
	ft_print(data, 3, id, get_current_time() - data->start_time);
	ft_usleep((data->time_to_eat > data->time_to_sleep) * (data->time_to_eat
			- data->time_to_sleep) + 1);
	return (0);
}

int	verif_sleeping(t_data *data, long time_to_sleep, int id)
{
	if (time_to_sleep >= data->time_to_die)
	{
		will_die(data, id);
		return (2);
	}
	if (stop_signal(data, data->philosophers[id].is_dead))
		return (2);
	ft_print(data, 2, id, get_current_time() - data->start_time);
	ft_usleep(time_to_sleep);
	return (0);
}

int	verif_eating(t_data *data, long time_to_eat, int id)
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
	ft_usleep(time_to_eat);
	direction_unlock_forks(data, id);
	return (0);
}

int	complet_routine(t_data *data, int id)
{
	if (data->time_to_die <= (data->time_to_eat))
	{
		will_die(data, id);
		return (2);
	}
	if (data->philosophers[id].id % 2 != 0)
		ft_usleep(data->time_to_eat);
	while (data->philosophers[id].repeat_meal_philo != 0)
	{
		if (data->philosophers[id].is_dead || stop_signal(data,
				data->philosophers[id].is_dead))
			break ;
		lock_forks(data, id);
		verif_eating(data, data->time_to_eat, id);
		verif_sleeping(data, data->time_to_sleep, id);
		verif_thinking(data, id);
		data->philosophers[id].repeat_meal_philo--;
	}
	direction_unlock_forks(data, id);
	return (0);
}

void	*philosopher_routine(void *philo)
{
	t_data_tmp	data_tmp;

	data_tmp = *(t_data_tmp *)philo;
	free(philo);
	if (data_tmp.data->nbr_philos == 1)
		routine_solo(data_tmp.data, data_tmp.id);
	else
		complet_routine(data_tmp.data, data_tmp.id);
	return (NULL);
}
