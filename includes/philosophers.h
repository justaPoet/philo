/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:50:01 by febouana          #+#    #+#             */
/*   Updated: 2024/10/04 23:04:06 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

typedef enum bool
{
	false,
	true
}			bool_t;

typedef struct philo_status
{
	pthread_t philo;         // OKOK
	pthread_mutex_t fork_l;  // OKOK
	pthread_mutex_t *fork_r; // OKOK
	bool_t left_locked;      // OKOK
	bool_t right_locked;     // OKOK
	bool_t is_dead;          // OKOK
	int repeat_meal_philo;   // OKOK
	long long last_meal;     // OKOK

}			philo_status_t;

typedef struct data
{
	philo_status_t *philosophers; // OKOK
	long nbr_philos;              // OKOK
	long time_to_die;             // OKOK
	long time_to_eat;             // OKOK
	long time_to_sleep;           // OKOK
	long repeat_meal;             // OKOK
	long long start_time;         // OKOK
	pthread_mutex_t m_print;      // OKOK
	pthread_mutex_t m_stop;       // OKOK
	bool_t stop;                  // OKOK
}			data_t;

typedef struct data_idx
{
	data_t *data; // program data
	int idx;      // Index of the current thread
}			data_idx_t;

//+ philosophers.c
void		join_philosophers(data_t *data);
void		create_philosophers(data_t *data);
void		assign_fork(data_t *data);
int			create_forks(data_t *data);

//+ philosophers_routine.c
int			verif_sleeping_thinking(data_t *data, long time_to_sleep, int id);
int			verif_eating(data_t *data, long time_to_eat, int id);
int			take_forks(data_t *data, int id, bool_t dead);
int			complet_routine(data_t *data, int id);
void		*philosopher_routine(void *index);

//+ philosophers_memento_mori.c
bool_t		stop_signal(data_t *data, bool_t dead);
void		will_die(data_t *data, int id);
int			routine_solo(data_t *data, int id);

//+ philosophers_utils.c
long long	get_current_time(void);
void		unlock_forks(data_t *data, int id);
int			lock_second_fork(data_t *data, int id, bool_t dead);
int			lock_first_fork(data_t *data, int id, bool_t dead);
int			print_all_action(data_t *data, int option, int id, long long time);

//+ gestion_errors.c
void		error_prompt(void);
void		error_forks(data_t *data);
void		destroy_fork(data_t *data);
int			verif_args(char **args);
int			parsing_args(data_t *data, int argc, char **args);

//+ libft_utils.c
long		ft_atol(const char *str);
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);

#endif
