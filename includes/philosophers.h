/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:50:01 by febouana          #+#    #+#             */
/*   Updated: 2024/10/08 20:49:13 by febouana         ###   ########.fr       */
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
}					bool_t;

typedef struct philo_status
{
	pthread_t		philo;
	pthread_mutex_t	fork_l;
	pthread_mutex_t	*fork_r;
	bool_t			left_locked;
	bool_t			right_locked;
	bool_t			is_dead;
	long long		last_meal;
	int				repeat_meal_philo;
	int				id;
}					philo_status_t;

typedef struct data
{
	philo_status_t	*philosophers;
	long			nbr_philos;
	long			time_to_die;
	long			time_to_eat;
	long			time_to_sleep;
	long			repeat_meal;
	long long		start_time;
	pthread_mutex_t	m_print;
	pthread_mutex_t	m_stop;
	bool_t			stop;
}					data_t;

typedef struct data_tmp
{
	data_t			*data;
	int				id;
}					data_tmp_t;

//+ philosophers.c
int					join_philosophers(data_t *data, int limit);
int					create_philosophers(data_t *data);
int					init_struct(data_t *data);
int					parsing_args(data_t *data, int argc, char **args);

//+ philosophers_routine.c
int					verif_thinking(data_t *data, int id);
int					verif_sleeping(data_t *data, long time_to_sleep, int id);
int					verif_eating(data_t *data, long time_to_eat, int id);
int					complet_routine(data_t *data, int id);
void				*philosopher_routine(void *index);

//+ philosophers_memento_mori.c
bool_t				stop_signal(data_t *data, bool_t dead);
void				will_die(data_t *data, int id);
int					routine_solo(data_t *data, int id);

//+ philosophers_utils.c
void				assign_fork(data_t *data);
int					create_forks(data_t *data);
long long			get_current_time(void);
void				good_ending(data_t *data);
int					ft_print(data_t *data, int option, int id, long long time);

//+ philosophers_forks.c
int					lock_forks(data_t *data, int id);
void				direction_unlock_forks(data_t *data, int id);
void				unlock_forks(data_t *data, int id);
void				unlock_forks_odd(data_t *data, int id);

//+ gestion_errors.c
void				error_prompt(void);
void				error_quit(data_t *data, int limit);
void				error_quit2(data_t *data, int limit);
int					destroy_fork(data_t *data, int limit);
int					verif_args(char **args);

//+ libft_utils.c
long				ft_atol(const char *str);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);

#endif
