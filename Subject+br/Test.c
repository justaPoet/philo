/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: febouana <febouana@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/08 18:55:52 by apoet             #+#    #+#             */
/*   Updated: 2024/09/11 14:32:32 by febouana         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/philosophers.h"


// int ft_waiter_checker()
// ==> verifie si les forks/spoons du philo passer en arg sont libres
// return 0 si le philo peut manger 1 si non

//+ Le pthread_join() free les threads ??? ==> confirme par gpt


//? VARIABLE NECESSAIRE DANS LA STRUCTURE
//? pthread_t philos[nbr_philo] == tableau contenant l'index de tous les threads
//? 

void philo_routine()
{
    printf("philo commence a gimgembre\n", "\e[1;33m");
    sleep(0);
    printf(">>>>>>>>>>>>>philo a fini de gimgembre\n");
}

// void create_spoons(data_t data, int nbr_philo)
// {
//     int i;
    
//     i = 0;
//     data.spoons[nbr_philo];
//     while (i < nbr_philo)
//         data.spoons[++i] = i + 1;  
//     //printf("SPOONS==%d\n", i);
// }

// int main()
// { 
//     int nbr_philo = 5;
//     //pthread_t philos[nbr_philo];
//     data_t data;
//     data.φ[nbr_philo];

//     if (nbr_philo == 0)
//         return (1);
//     // else
//     //     create_spoons(data, nbr_philo);
    
//     int i = 0;
//     while (i < nbr_philo)
//     {
//         if (pthread_create(&data.φ[i], NULL, philo_routine, NULL) != 0)
//             return (1);
//         // if (pthread_create(&philos[i], NULL, philo_routine, NULL) != 0)
//         //     return (1);
//         // if (pthread_join(philos[i], NULL) != 0) //? Possibilite d'integrer directement la condition pthread_join juste apres pthread_create si l'objectif est de sequencer l'ensemble des threads, qu'ils passent chacun leur tour, mais pas utile pour nous ici     
//         //     return (1);
//         printf("main : philo(%d) created\n", i + 1);            
//         i++;
//     }
//     i = 0;
//     // while (i < nbr_philo) //? boucle avec pthread_join necessaire si multithreads PAR CONTRE possibilite d'integrer directement la condition pthread_join juste apres pthread_create si l'objectif est de sequencer l'ensemble des threads, qu'ils passent chacun leur tour, mais pas utile pour nous ici
//     // {
//     //     if (pthread_join(philos[i], NULL) != 0)
//     //         return (1);
//     //     printf("main : philo(%d) joined\n", i + 1);    
//     //     i++;
//     // }
//     return (0);
// }


//!====================================================================================================================================================

// memset == a utiliser au moment d'initialiser une variable pour passer chaque valeur a 0 et ainsi eviter les possibles erreurs 
// gettimeofday == sert a lire ou programmer l'heure ainsi que le fuseau horaire //+ => va nous premettre de gerer les limites de temps imposes par le sujet 

// pthread_mutex_init == permet d'inistialiser un mutex //+ => va nous permettre de verrouiller les spoons quand on en aura besoin et aussi pour eviter les deadlocks
// pthread_mutex_destroy == permet de detruire le mutex de poser //+
// pthread_mutrex_lock == permet de verrouille la variable precisee //+ 
// pthread_mutex_unlock == permet de deverouille la variable precisee precedemment bloque //+

// pthread_create == permet de creer un thread lorsqu'on lui donne une variable de type pthread_t //+ permet de creer chaque philosophe sous forme de thread
// pthread_detach == permet de placer un thread en etat "detache". Cela garantit que les ressources memoires consommees seront immediatement liberees lorsque l'execution s'achevera> En gros elle ne depend pas de pthread_join(), d'etre ratache au thread pere pour liberer les ressources une fois fini. //? utile en fin de programme pour eviter les leaks ?????
// pthread_join == permet de mettre en suspend l'execution du thread appelant jusqu'a ce que le thread identifie par le premier arg acheve son execution //? ==> sorte de wait() mais pour les threads

//? NB : il existe deux etats pour un thread, joignable et injoignable. 
//? Il devient injoignable a cause de pthread_detach(),
//? et ainsi prhtread_join() ne peux plus etre utilise par exemple.


//==============







//+> Creer le systeme des spoons (+ la structure)
    //! systeme de spoon doit etre un char* pareil que φ[] ??? 
//+> Creer une fonction waiter_checker(), qui verifie les spoons a gauche et a droite du thread precise en arg 
//+  pour donner l'autorisation a ce meme thread/philo s'il peut manger ou pas encore dans le cas ou la spoon est deja utilisee par un autre thread/philo (//eviter les deadlocks)
    //! comment verifier si une spoon est deja utilisee ? ==> grace aux fn pthreads_mutex() ???
//+> La routine de philo (manger, penser, dormir) doit tenir en "une fonction", et elle doit faire appel dans cette meme fonction au waiter_checker()
    