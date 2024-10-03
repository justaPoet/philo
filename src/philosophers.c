/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 14:49:20 by febouana          #+#    #+#             */
/*   Updated: 2024/10/03 20:57:24 by febouana         ###   ########.fr       */
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
            return ;   //! GESTION ERROR   
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
        
    pthread_mutex_init(&data->print, NULL);
    pthread_mutex_init(&data->will_die, NULL);
    pthread_mutex_init(&data->m_stop, NULL); //!
    
    while (i < data->nbr_philos)
    {
        data->philosophers[i].repeat_meal_philo = data->repeat_meal; 
        data->philosophers[i].last_meal = 0; 
        data->philosophers[i].last_last_meal = 0;
        data->philosophers[i].id = i; //id pour print mort
        data->philosophers[i].is_dead = false;

        data->philosophers[i].right_locked = false;
        data->philosophers[i].right_locked = false;
        
        data_idx_t *data_idx = malloc(sizeof(*data_idx));
        data_idx->data = data;
        data_idx->idx = i;    
        
        if (pthread_create(&(data->philosophers[i].philo), NULL, &philosopher_routine, data_idx) != 0)
            return ;
        i++;
    }   
   // checkcheck_death(data);
}

//? OKOK
//? assigne la bonne fork pour chaque philo + assigne au dernier 
//? la fork du premier pour faire la boucle 
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

//? OKOK
//? creer le bon nombre de fork
int create_forks(data_t *data)
{
    int i;
    
    i = 0;
    while (i < data->nbr_philos)
    {
        if (pthread_mutex_init(&(data->philosophers[i].fork_l), NULL) != 0)
        {
            //error_forks(&data); //! peux possiblement free des fork meme pas allouer et segments
            return (2);
        }
        i++;
    }   
    assign_fork(data);
    return (0);
}

int parsing_args(data_t *data, int argc, char **args)
{
    data->nbr_philos = ft_atol(args[1]);
    if (data->nbr_philos > 200 || data->nbr_philos == 0)
    {
        error_prompt();
        return (2); 
    }
    data->time_to_die = ft_atol(args[2]);
    data->time_to_eat = ft_atol(args[3]);
    data->time_to_sleep = ft_atol(args[4]);

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
    
    // if (data.stop == false && data.repeat_meal > 0)
    //     printf("\nEach philosopher ate %ld time(s) 🏆\n", data.repeat_meal);
    // if (data.stop)
    //     printf("%lld ☠️  (%d) IS DEAD\n", get_current_time() - data.start_time, data.id_philo_dead + 1); //!
    
    //pthread_mutex_destroy(&data->eating);
    pthread_mutex_destroy(&data.print);    
    //pthread_mutex_destroy(&data->repeat_eat);    
    //pthread_mutex_destroy(&data->will_eating);
    free(data.philosophers);
}






























// Plus d'erreurs qd j'initie ma struct... ok?
// void init_vars(data_t *data)
// {
//     data->nbr_philos = 0; 
// 	data->time_to_die = 0;
// 	data->time_to_eat = 0;
// 	data->time_to_sleep = 0;
// 	data->repeat_meal = -1;
// 	data->start_time = 0;
// }


//! complique pour rien
// bool_t everyone_is_full(data_t *data)
// {
//     int i;
//     int good_ending;
    
//     i = 0;
//     good_ending = 0;
//     while (i < data->nbr_philos)
//     {
//         if (data->philosophers[i].is_full == true)
//             good_ending++;   
//         i++;
//     } 
//     if (i == good_ending)
//         return (true);
//     else 
//         return (false);
// }

// void checkcheck_death(data_t *data)
// {
//     int i;
//     i = 0;
    
//     while(1)
//     {
//         if (i == data->nbr_philos)
//             i = 0;
//         if (data->philosophers[i].is_dead == true)
//         {
//             pthread_mutex_lock(&data->m_stop);
//             data->stop = true;
//             pthread_mutex_lock(&data->m_stop);
//             print_all_action(data, 5, i, get_current_time() - data->start_time, data->philosophers[i].is_dead); //? safe
//             // sleep(5);
//             // exit (2); //!
//             return ;
//         }
//         // if (everyone_is_full(data))
//         //     return ;
//         i++;
//     }   
// }
