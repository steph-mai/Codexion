/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stephanie <stephanie@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 11:56:02 by stephanie         #+#    #+#             */
/*   Updated: 2026/04/30 12:24:15 by stephanie        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	validate_args(t_args *args)
{
	// une fois qu'on a rempli la structure on verifie que la logique métier est respectee
}

int	fill_args_structure(t_data *data, int argc,char **argv)
{
	// verifier le nb d'arg
	// 1. Boucle pour vérifier is_number_str sur tous les arguments numériques
    // 2. Remplissage : args->t_burnout = atoi(argv[1]); ...
    // 3. Gestion du scheduler (strcmp avec "fifo" ou "edf")
	// 4 appeler validate_args et return son resultat
}

static int	is_number_str(char *s)
{
	// Si s est vide ou nul -> return 0
	// Boucle sur s[i] : si s[i] n'est pas un chiffre -> return 0
	// return 1
}

