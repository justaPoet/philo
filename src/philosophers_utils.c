/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/30 17:30:39 by febouana          #+#    #+#             */
/*   Updated: 2024/10/07 00:29:54 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void	eventually_unlock_forks(data_t *data, int id)
{
	if (data->philosophers[id].right_locked)
	{
		pthread_mutex_unlock(data->philosophers[id].fork_r);
		data->philosophers[id].right_locked = false;
	}
	if (data->philosophers[id].left_locked)
	{
		pthread_mutex_unlock(&data->philosophers[id].fork_l);
		data->philosophers[id].left_locked = false;
	}
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

int	print_all_action(data_t *data, int option, int id, long long time)
{
	char	*actions[5] = {"%ld 🍴 (%d) has taken a fork\n",
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
// int	print_all_action(data_t *data, int option, int id, long long time)
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
