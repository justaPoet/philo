/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:49:20 by febouana          #+#    #+#             */
/*   Updated: 2024/10/08 20:48:15 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

int	join_philosophers(data_t *data, int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		if (pthread_join(data->philosophers[i].philo, NULL) != 0)
		{
			error_quit(data, limit);
			return (2);
		}
		i++;
	}
	return (0);
}

int	create_philosophers(data_t *data)
{
	int			i;
	data_tmp_t	*data_tmp;

	i = 0;
	while (i < data->nbr_philos)
	{
		data_tmp = malloc(sizeof(*data_tmp));
		if (!data_tmp)
		{
			error_quit2(data, i);
			return (2);
		}
		data_tmp->data = data;
		data_tmp->id = i;
		if (pthread_create(&(data->philosophers[i].philo), NULL,
				&philosopher_routine, data_tmp) != 0)
		{
			error_quit2(data, i);
			return (2);
		}
		i++;
	}
	return (0);
}

int	init_struct(data_t *data)
{
	int	i;

	i = 0;
	data->start_time = get_current_time();
	if (data->start_time == -1)
	{
		error_quit(data, data->nbr_philos);
		return (2);
	}
	data->stop = false;
	pthread_mutex_init(&data->m_print, NULL);
	pthread_mutex_init(&data->m_stop, NULL);
	while (i < data->nbr_philos)
	{
		data->philosophers[i].left_locked = false;
		data->philosophers[i].right_locked = false;
		data->philosophers[i].is_dead = false;
		data->philosophers[i].last_meal = 0;
		data->philosophers[i].repeat_meal_philo = data->repeat_meal;
		data->philosophers[i].id = i + 1;
		i++;
	}
	return (0);
}

int parsing_args(data_t *data, int argc, char **args)
{
    data->nbr_philos = ft_atol(args[1]);
    data->time_to_die = ft_atol(args[2]);
    data->time_to_eat = ft_atol(args[3]);
    data->time_to_sleep = ft_atol(args[4]);
    if (data->nbr_philos > 200 || data->nbr_philos == 0 || data->time_to_die < 60 || data->time_to_eat < 60 || data->time_to_sleep < 60)
    {
        error_prompt();
        return (2); 
    }
    if (argc == 6)
    {
        data->repeat_meal = ft_atol(args[5]);
        if (data->repeat_meal == 0)
        {
            error_prompt();
            return (2); 
        }
    }
    else
        data->repeat_meal = -1;
    return (0);
}

//$> ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] ([number_of_times_each_philosopher_must_eat])

int	main(int argc, char **argv)
{
	data_t	data;

	if (argc < 5 || 6 < argc)
		return (2);
	if (verif_args(argv) == 2)
		return (2);
	if (parsing_args(&data, argc, argv) == 2)
		return (2);
	data.philosophers = malloc(data.nbr_philos * sizeof(philo_status_t));
	if (!data.philosophers)
		return (2);
	if (init_struct(&data) == 2)
		return (2);
	if (create_forks(&data) == 2)
		return (2);
	if (create_philosophers(&data) == 2)
		return (2);
	if (join_philosophers(&data, data.nbr_philos) == 2)
		return (2);
	good_ending(&data);
}
