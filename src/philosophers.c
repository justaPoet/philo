/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:49:20 by febouana          #+#    #+#             */
/*   Updated: 2024/10/04 23:17:07 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

void join_philosophers(data_t *data)
{
    int i;
    
    i = 0;
    while (i < data->nbr_philos)
    {
        if (pthread_join(data->philosophers[i].philo, NULL) != 0)
        {
            printf("BAMBPOCLAT join philo\n");
            return ;   //! gestion error
        }
        i++;
    } 
}

void create_philosophers(data_t *data)
{
    int i;
    i = 0;

    data->stop = false; 
    data->start_time = get_current_time(); //?safe
    if (data->start_time == -1)
        return ; //! gestion error      
    pthread_mutex_init(&data->m_print, NULL);
    pthread_mutex_init(&data->m_stop, NULL);
    while (i < data->nbr_philos)
    {
        data->philosophers[i].repeat_meal_philo = data->repeat_meal; 
        data->philosophers[i].last_meal = 0; 
        data->philosophers[i].is_dead = false;
        data->philosophers[i].left_locked = false;
        data->philosophers[i].right_locked = false;
        
        data_idx_t *data_idx = malloc(sizeof(*data_idx));
        data_idx->data = data;
        data_idx->idx = i;    
        
        if (pthread_create(&(data->philosophers[i].philo), NULL, &philosopher_routine, data_idx) != 0)
            return ;
        i++;
    }   
}

void assign_fork(data_t *data)
{
    int i;

    i = 0;
    while (i < data->nbr_philos - 1)
    {
        data->philosophers[i].fork_r = &data->philosophers[i + 1].fork_l;
        i++;
    }
    data->philosophers[i].fork_r = &data->philosophers[0].fork_l;
} 

int create_forks(data_t *data)
{
    int i;
    
    i = 0;
    while (i < data->nbr_philos)
    {
        if (pthread_mutex_init(&(data->philosophers[i].fork_l), NULL) != 0)
        {
            //error_forks(&data); //! peux possiblement free des fork meme pas allouer et segments //? envoyer "i" et destroy jusqu'a sa valeur
            return (2);
        }
        i++;
    }   
    assign_fork(data);
    return (0);
}

// $> ./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] ([number_of_times_each_philosopher_must_eat])

int main(int argc, char **argv)
{     
    data_t data;
    
    if (argc < 5 || 6 < argc)
        return (2);
    if (verif_args(argv) == 2)
        return (2);
    if (parsing_args(&data, argc, argv) == 2)
        return (2);
    data.philosophers = malloc(data.nbr_philos * sizeof(philo_status_t));;
    if (!data.philosophers)
    {
        error_prompt();
        return (2);
    }
    if (create_forks(&data) == 2)
        return (2);
    create_philosophers(&data);
    join_philosophers(&data);
    destroy_fork(&data);
    if (data.stop == false && data.repeat_meal > 0)
        printf("\nEach philosopher ate %ld time(s) 🏆\n", data.repeat_meal);
    pthread_mutex_destroy(&data.m_print);  
    pthread_mutex_destroy(&data.m_stop);    
    free(data.philosophers);
}










//+ TODOLIST :
//+ mieux opti les cas de routine infini (5 500 200 200 | 4 410 200 200)
    //!if nbr_philos == impair => faire sleep les pair plutot qu'eux et inversement 
        //! rajouter condition avant de boucler pour faire sleep les bons...
        //! ... et rajouter un if dans lock_forks (et unlock_forks ?) pour mieux opti aussi du coup
    //+ remettre bool_t dead ?
    //+ pour micro optimiser le calcul final : mettre un tri baleze que pour le premier passage, pas besoin pour ceux d'apres et trop gourmand ??
//+ refaire destroy_forks()
//+ finir la gestion d'erreurs (+ refaire au propre les init ?)
//+ ((norminette))
