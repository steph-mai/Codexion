/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_tasks.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/06 11:45:09 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/06 12:07:24 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "codexion.h"

int take_available_dongles(t_coder *coder)
{
    t_dongle *first;
    t_dongle *second;

    if (coder->left_dongle->id < coder->right_dongle->id)
    {
        first = coder->left_dongle;
        second = coder->right_dongle;
    }
    else
    {
        first = coder->right_dongle;
        second = coder->left_dongle;
    }
	if (!lock_mutex_and_take_dongles(coder, first, second))
    	return (0);
	return (1);
}

int	lock_mutex_and_take_dongles(t_coder *coder, t_dongle *first, t_dongle *second)
{
	pthread_mutex_lock(&first->dongle_mutex);
	if (!get_simulation_status(coder->data))
	{
		pthread_mutex_unlock(&first->dongle_mutex);
		return (0);
	}
	
    print_status(coder, "has taken a dongle");
    if (coder->left_dongle == coder->right_dongle) //si tout seul!
    {
        usleep(coder->data->parsed_args.time_to_burnout * 1000);
        pthread_mutex_unlock(&first->dongle_mutex);
        return (0);
    }
    pthread_mutex_lock(&second->dongle_mutex);
	if (!get_simulation_status(coder->data))
	{
		pthread_mutex_unlock(&first->dongle_mutex);
		pthread_mutex_unlock(&second->dongle_mutex);
		return (0);
	}
    print_status(coder, "has taken a dongle");
	return (1);
}

void compilation_work(t_coder *coder)
{
	print_status(coder, "is compiling");
	pthread_mutex_lock(&coder->data->simulation_over_mutex);
	coder->last_compilation_time = get_time_ms();
	coder->achieved_compilations_nb++;
	pthread_mutex_unlock(&coder->data->simulation_over_mutex);
	smart_sleep(coder->data->parsed_args.time_to_compile, coder->data);
}

void put_dongles_away(t_coder *coder)
{
	pthread_mutex_unlock(&coder->left_dongle->dongle_mutex);
	pthread_mutex_unlock(&coder->right_dongle->dongle_mutex);
}

int complete_tasks(t_coder *coder)
{
	print_status(coder, "is debugging");
	smart_sleep(coder->data->parsed_args.time_to_debug, coder->data);
	print_status(coder, "is refactoring");
	smart_sleep(coder->data->parsed_args.time_to_refactor, coder->data);
	return (get_simulation_status(coder->data));
}

