/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_dongles.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 12:08:20 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/06 13:38:25 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int	is_dongle_cooldown_finished(t_dongle *dongle, t_data *data)
{
    long long timestamp;

   	timestamp = get_time_ms() - data->start_time;
    if (current_time - dongle->last_used_time >= data->parsed_args.time_to_cooldown)
        return (1);
    return (0);
}
int is_dongle_ready(t_dongle *dongle, t_data *data)
{
    if (dongle->is_available && is_dongle_cooldown_finished(dongle, data))
        return (1);
    return (0);
}
void put_dongles_away(t_coder *coder)
{
    long long now = get_time_ms();
    coder->left_dongle->last_used_time = now;
    coder->right_dongle->last_used_time = now;

    pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
    pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
}
