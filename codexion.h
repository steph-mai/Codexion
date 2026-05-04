/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 10:21:52 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/04 11:59:13 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <limits.h>
# include <sys/time.h>

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}				t_scheduler;

typedef struct s_data
{
		t_args			parsed_args;
		t_dongle		*dongles;
		t_coder			*coders;
		pthread_mutex_t	print_mutex;
		pthread_mutex_t simulation_over_mutex;
		pthread_t		thread_monitoring;
		long long			start_time;
		size_t 			is_burn_out;
		size_t			is_simulation_a_success;
}				t_data;

typedef struct s_args
{
	size_t			number_of_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	size_t			number_of_compiles_required;
	long long		dongle_cooldown;
	t_scheduler	scheduler;
}				t_args;

typedef struct s_dongle
{
	pthread_mutex_t	dongle_mutex;
	pthread_cond_t	dongle_cond;
	int				id; // essentiels de hierarchiser (tjs d'abord la clé avec le plus petit ID) pour éviter blocages (Deadlocks)
	long long			available_at;
	int				is_available;
}				t_dongle;

typedef struct s_coder
{
	size_t 		id;
	pthread_t	thread_id;
	long long 		last_compilation_time;
	size_t		achieved_compilations_nb;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_data 		*data;
}				t_coder;

// ######################## PARSING AND INITIZLIZATION ########################
int		validate_args(t_args *args);
int		fill_args_structure(t_data *data, int argc,char **argv);

// ######################## UTILS AND FREE ########################
int		free_everything_and_return(t_data *data);
void	cleanup_simulation(t_data *data);

// ######################## THREADS ########################
void	*coder_routine(void *arg);
int 	take_available_dongles(t_coder *coder);
void	compilation_work(t_coder *coder);
void	put_dongles_away(t_coder *coder);
int		complete_tasks(t_coder *coder);

// ######################## SIMULATION #######################
int		start_simulation(t_data *data);
void	set_simulation_stop(t_data *data);

// ######################## SCHEDULER #######################
long    get_deadline(t_coder *coder);
int     is_dongle_ready(t_dongle *dongle);

// ######################## MONITORING #######################
void    *monitor_routine(void *arg);
int     is_burn_out(t_data *data);      // Vérifie le flag sécurisé
int     check_all_alive(t_data *data);  // Le monitor vérifie chaque last_compilation_time

// ######################## UTILS #######################
long 	get_time_ms(void);
int 	is_simulation_over(t_data *data);
void 	print_status(t_coder *coder, char *msg);

#endif
