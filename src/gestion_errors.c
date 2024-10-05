/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gestion_errors.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/16 19:38:53 by febouana          #+#    #+#             */
/*   Updated: 2024/10/04 22:29:01 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void error_prompt()
{
    ft_putstr_fd("\033[31m$> ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] ([number_of_times_each_philosopher_must_eat])\033[0m\n", 2);
}

void error_forks(data_t *data)
{
	destroy_fork(data); //! peux possiblement free des fork meme pas allouer et segment
	free(data->philosophers);
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
