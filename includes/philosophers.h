/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:50:01 by febouana          #+#    #+#             */
/*   Updated: 2024/10/11 22:35:01 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum bool
{
	false,
	true
}					t_bool;

typedef struct philo_status
{
	pthread_t		philo;
	pthread_mutex_t	fork_l;
	pthread_mutex_t	*fork_r;
	t_bool			left_locked;
	t_bool			right_locked;
	t_bool			is_dead;
	long long		last_meal;
	int				repeat_meal_philo;
	int				id;
}					t_philo_status;

typedef struct data
{
	t_philo_status	*philosophers;
	long			nbr_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			repeat_meal;
	long long		start_time;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_stop;
	t_bool			stop;
}					t_data;

typedef struct t_data_tmp
{
	t_data			*data;
	int				id;
}					t_data_tmp;

//+ philosophers.c
int					join_philosophers(t_data *data, int limit);
int					create_philosophers(t_data *data);
int					init_struct(t_data *data);
int					parsing_args(t_data *data, int argc, char **args);

//+ philosophers_routine.c
int					verif_thinking(t_data *data, int id);
int					verif_sleeping(t_data *data, long time_to_sleep, int id);
int					verif_eating(t_data *data, long time_to_eat, int id);
int					complet_routine(t_data *data, int id);
void				*philosopher_routine(void *index);

//+ philosophers_memento_mori.c
t_bool				stop_signal(t_data *data, t_bool dead);
void				will_die(t_data *data, int id);
int					routine_solo(t_data *data, int id);

//+ philosophers_utils.c
long long			get_current_time(void);
void				ft_usleep(long long int time);
void				assign_fork(t_data *data);
void				good_ending(t_data *data);
int					ft_print(t_data *data, int option, int id, long long time);

//+ philosophers_forks.c
int					create_forks(t_data *data);
int					lock_forks(t_data *data, int id);
void				direction_unlock_forks(t_data *data, int id);
void				unlock_forks(t_data *data, int id);
void				unlock_forks_odd(t_data *data, int id);

//+ gestion_errors.c
void				error_prompt(void);
void				error_quit(t_data *data, int limit);
void				error_quit2(t_data *data, int limit);
int					destroy_fork(t_data *data, int limit);
int					verif_args(char **args);

//+ libft_utils.c
long				ft_atol(const char *str);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);

long long			get_current_time(void);
t_bool				ft_get_time(long *time);

#endif
