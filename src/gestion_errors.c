/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:38:53 by febouana          #+#    #+#             */
/*   Updated: 2024/10/08 21:17:44 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	error_prompt(void)
{
	ft_putstr_fd("\033[31m$> ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] \
	 ([number_of_times_each_philosopher_must_eat])\033[0m\n", 2);
}

void	error_quit(t_data *data, int limit)
{
	destroy_fork(data, limit);
	pthread_mutex_destroy(&data->m_print);
	pthread_mutex_destroy(&data->m_stop);
	free(data->philosophers);
	ft_putstr_fd("\033[31mError\033[0m\n", 2);
}

void	error_quit2(t_data *data, int limit)
{
	destroy_fork(data, limit);
	pthread_mutex_destroy(&data->m_print);
	pthread_mutex_destroy(&data->m_stop);
	free(data->philosophers);
	join_philosophers(data, limit);
	ft_putstr_fd("\033[31mError\033[0m\n", 2);
}

int	destroy_fork(t_data *data, int limit)
{
	int	i;

	i = 0;
	while (i < limit)
	{
		if (pthread_mutex_destroy(&data->philosophers[i].fork_l) != 0)
			return (2);
		i++;
	}
	return (0);
}

int	verif_args(char **args)
{
	int	x;
	int	i;

	x = 1;
	while (args[x])
	{
		i = 0;
		while (args[x][i])
		{
			if (!(args[x][i] >= '0' && args[x][i] <= '9'))
			{
				error_prompt();
				return (2);
			}
			i++;
		}
		x++;
	}
	return (0);
}
