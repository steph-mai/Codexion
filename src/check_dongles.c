/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dongles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 12:08:20 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/06 14:25:02 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

static int	is_dongle_cooldown_finished(t_dongle *dongle, t_data *data)
{
	pthread_mutex_lock(&dongle->dongle_mutex);
	if (&dongle->available_at >= get_timestamp(data))
        return (1);
    return (0);
}
int is_dongle_ready(t_dongle *dongle, t_data *data)
{
    if (dongle->is_available && is_dongle_cooldown_finished(dongle, data))
        return (1);
    return (0);
}

