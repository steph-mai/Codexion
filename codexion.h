/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/07 10:03:04 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/18 14:18:45 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>
# include <limits.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_data	t_data;

typedef enum e_scheduler
{
	SCHEDULER_FIFO,
	SCHEDULER_EDF
}				t_scheduler;

typedef struct s_args
{
	size_t			number_of_coders;
	long long		time_to_burnout;
	long long		time_to_compile;
	long long		time_to_debug;
	long long		time_to_refactor;
	size_t			number_of_compiles_required;
	long long		dongle_cooldown;
	t_scheduler		scheduler;
}				t_args;

typedef struct s_node
{
	size_t		coder_id;
	long long	priority_marker;
	long long	arrival_time;
	void		*left_dongle;
	void		*right_dongle;
}	t_node;

typedef struct s_queue
{
	t_node		*node;
	size_t		size;
}	t_queue;

typedef struct s_dongle
{
	size_t			id;
	long long		available_at;
	int				is_unused;
}					t_dongle;

typedef struct s_coder
{
	size_t		id;
	pthread_t	thread_id;
	long long	last_compilation_time;
	size_t		achieved_compilations_nb;
	t_dongle	*left_dongle;
	t_dongle	*right_dongle;
	t_data		*data;
}				t_coder;

typedef struct s_data
{
	t_args			parsed_args;
	t_dongle		*dongles;
	t_coder			*coders;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	simulation_mutex;
	pthread_t		thread_monitoring;
	long long		start_time;
	int				is_simulation_running;
	t_queue			wait_queue;
	pthread_mutex_t	queue_mutex;
	pthread_cond_t	queue_cond;
}				t_data;

// ######################## PARSING AND INITIALIZATION ########################
int			validate_args(t_args *args);
int			fill_args_structure(t_args *args, int argc, char **argv);
int			init_data(t_data *data);

// ######################## UTILS AND FREE ########################
int			free_everything_and_return(t_data *data, int exit_code);
void		cleanup_simulation(t_data *data);

// ######################## CODER THREADS ########################
void		*coder_routine(void *arg);
int			identify_and_take_dongles(t_coder *coder);
void		put_dongles_away(t_coder *coder);

// ######################## SIMULATION #######################
int			start_simulation(t_data *data);
void		set_simulation_stop(t_data *data);
int			get_simulation_status(t_data *data);

// ######################## SCHEDULER #######################
int			is_priority(t_node a, t_node b);
void		push_in_wait_queue(t_queue *wait_queue, t_node new_in_wait_list);
void		wait_queue_pop(t_queue *wait_queue);
void		remove_from_wait_queue(t_queue *wait_queue, size_t coder_id);
size_t		get_first_in_wait_queue(t_queue *wait_queue);

// ######################## HEAP UTILS #######################
void		swap_nodes(t_node *a, t_node *b);
size_t		get_smallest_child(t_queue *wait_queue, size_t index);

// ######################## MONITORING #######################
void		*monitor_routine(void *arg);

// ######################## TIMER #######################
void		active_sleep(long long time_to_wait, t_data *data);
long long	get_time_ms(void);
long long	get_timestamp(t_data *data);

// ######################## PRINT #######################
void		print_status(t_coder *coder, char *msg);
int			print_error(char *message);
int			print_use(void);

#endif
