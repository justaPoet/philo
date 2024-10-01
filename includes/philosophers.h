/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:50:01 by febouana          #+#    #+#             */
/*   Updated: 2024/10/01 17:14:45 by febouana         ###   ########.fr       */
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
}					bool_t;

typedef struct philo_status
{
	pthread_mutex_t		mutex;

	pthread_t			philo; //OKOK
	int					philo_id; //OKOK ==> id 
	int					repeat_meal_philo; //OKOK
	pthread_mutex_t		fork_l; //OKOK
	pthread_mutex_t		*fork_r; //OKOK
    bool_t 				left_locked; //OKOK
    bool_t 				right_locked; //OKOK	
	
	long long			last_meal;
	long long			last_last_meal; //changer 

	long long start_time;



	bool_t is_dead;

}					philo_status_t;

typedef struct data
{
	philo_status_t	*philosophers; //OKOK
	long			nbr_philos; //OKOK
	long			time_to_die; //OKOK
	long			time_to_eat; //OKOK
	long			time_to_sleep; //OKOK
	long			repeat_meal; //OKOK
	long long		start_time; //OKOK

	pthread_mutex_t print; //OKOK

	pthread_mutex_t mutex; //OKOK

	//pthread_mutex_t will_die; //! raf?
		
	bool_t			stop; //+ le monitor previens les philos de stopper leurs routines
	bool_t			good_ending; //+ on dit au monitor de s'arreter car good_ending

	
	int 			id_philo_dead;
	long long 		time_death;		
}					data_t;

//+ philosophers.c
void join_philosophers(data_t data);
//
//

//+ philosophers_routine.c
int 				verif_eating(data_t *data, long obj_usleep, int id, bool_t status);
int 				verif_sleeping(data_t *data, long obj_usleep, int id, bool_t status);
int 				complet_routine(data_t *data, int id);
void				*philosopher_routine(void *index);

//+ philosophers_utils.c
data_t				*get_data(void);
void				destroy_fork(data_t data);

int  				check_death(data_t *data, int id);
int  				check_death_solo(data_t *data, int id);
long long			get_current_time(void);

int print_action2(data_t *data, long long time, char *emoji, char *action, int id, bool_t status);

//+ gestion_errors.c
void				error(void);
void				error_prompt(void);
void				error_forks(data_t *data);
void				good_ending(data_t *data);
int					verif_args(char **args);

//+ libft_utils.c
long				ft_atol(const char *str);
void				ft_putchar_fd(char c, int fd);
void				ft_putstr_fd(char *s, int fd);

void				unlock_forks(data_t *data, int id);


int lock_first_fork(data_t *data, int id, bool_t status);
int lock_second_fork(data_t *data, int id, bool_t status);

int	print_all_action(data_t *data, int option, int id, long long time, bool_t status);

#endif
