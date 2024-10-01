/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/29 15:45:40 by febouana          #+#    #+#             */
/*   Updated: 2024/09/30 19:02:20 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philosophers.h"

// int everyone_is_full(data_t data)
// {
//     int counter;
//     int i;
    
//     counter = 0;
//     i = 0;
//     while (i < data.nbr_philos)
//     {
//         if (pthread_mutex_init(&data.philosophers[i].fork_l, NULL) != 0)
//         {
//             return (2);
//         }
//         i++;
//     }   
// }

// void monitor_checker()
// {
//     int i;
//     i = 0;
    
//     data_t *data;
//     data = get_data();
    
//     while(1)
//     {
//         if (i == data->nbr_philos)
//             i = 0;
//         if (data->philosophers[i].is_dead == true)
//         {
//             data->stop = true;
//             printf("MONITOR : A PHILO IS DEAD\n");
//             exit (2); //!
//         }
//         // everyone_is_full();
//         i++;
//     }   
// }

// faire boucler sur chaque philo et regarder l'etat pour savoir quoi faire 
// si rien ==> reboucle 
// si is_dead ==> fait stopper proprement le programme
