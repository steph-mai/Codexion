/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 16:19:38 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/05 09:22:04 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// Pour EDF (Earliest Deadline First)
long long    get_deadline(t_coder *coder)
{

	(void)coder;
	return (0);
}

// Pour le Cooldown (long dongle_cooldown)
int     is_dongle_ready(t_dongle *dongle)
{
	(void)dongle;
	return(0);
}
