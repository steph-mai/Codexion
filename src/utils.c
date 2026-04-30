/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 16:45:57 by stephanie         #+#    #+#             */
/*   Updated: 2026/04/30 16:51:23 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

//fonctions utilisées par tous les fichiers

int is_simulation_over(t_data *data)
{
	//action : Lecture : Elle renvoie la valeur actuelle du flag.
	// utilisee par tous les threads (Codeurs et Monitor).
	//Utilisee Tout le temps (à chaque itération de boucle).
	//se fait sous protection d'un mutex aussi
	//car si la varaible etait lue et modifiee en même temps ca pourrait crasher !
}
long get_time_ms(void)
{

}
void print_status(t_coder *coder, char *msg)
{
	//centralise et sécurise tous les messages de log du programme.
	//Synchronisation (Mutex) :  Sans le print_mutex, leurs messages printf s'entremêleraient dans la console
	//Horodatage (Timestamp) : Elle calcule automatiquement le temps écoulé depuis le début de la simulation
	//(get_time_ms() - start_time). C'est ce qui permet d'afficher : [1500ms] Coder 3 is compiling.
	// Contrôle de fin : Elle vérifie si la simulation est finie avant d'afficher. Cela évite qu'un codeur n'affiche "I'm working"
	//alors qu'un autre vient de mourir il y a 1ms.
	//On fait une exception pour le message "died from burnout" car c'est le message final.
	//Qui l'appelle ?
	//Absolument tout le monde (sauf le main). Elle est parsemée dans le code pour suivre l'évolution de l'état des codeurs.
}

