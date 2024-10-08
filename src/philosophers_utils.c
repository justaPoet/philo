/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:30:39 by febouana          #+#    #+#             */
/*   Updated: 2024/10/08 21:17:44 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	assign_fork(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nbr_philos - 1)
	{
		data->philosophers[i].fork_r = &data->philosophers[i + 1].fork_l;
		i++;
	}
	data->philosophers[i].fork_r = &data->philosophers[0].fork_l;
}

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

long long	get_current_time(void)
{
	struct timeval	tv;
	long long		milliseconds;

	if (gettimeofday(&tv, NULL) == -1)
		return (-1);
	milliseconds = (tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL);
	return (milliseconds);
}

void	good_ending(t_data *data)
{
	if (data->stop == false && data->repeat_meal > 0)
		printf("\nEach philosopher ate %ld time(s) 🏆\n", data->repeat_meal);
	destroy_fork(data, data->nbr_philos);
	pthread_mutex_destroy(&data->m_print);
	pthread_mutex_destroy(&data->m_stop);
	free(data->philosophers);
}

int	ft_print(t_data *data, int option, int id, long long time)
{
	const char	*actions[5] = {"%ld 🍴 (%d) has taken a fork\n",
		"%ld 🍝 (%d) is eating\n", "%ld 💤 (%d) is sleeping\n",
		"%ld 🤔 (%d) is thinking\n", "%ld ☠️  (%d) died\n"};

	pthread_mutex_lock(&data->m_print);
	if (data->philosophers[id].is_dead)
	{
		pthread_mutex_unlock(&data->m_print);
		return (2);
	}
	printf(actions[option], time, id + 1);
	pthread_mutex_unlock(&data->m_print);
	return (0);
}

//? Philosophers visualizer
// int	ft_print(t_data *data, int option, int id, long long time)
// {
//     pthread_mutex_lock(&data->m_print);
//     if (data->philosophers[id].is_dead)
//     {
//         pthread_mutex_unlock(&data->m_print);
//         return (2);
// 	}
//     char	*actions[5] = {
// 		"%ld %d has taken a fork\n",
// 		"%ld %d is eating\n",
// 		"%ld %d is sleeping\n",
// 		"%ld %d is thinking\n",
// 		"%ld %d died\n"};

// 	printf(actions[option], time, id + 1);
//     pthread_mutex_unlock(&data->m_print);
//     return (0);
// }
