/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/05 11:52:49 by stmaire           #+#    #+#             */
/*   Updated: 2026/05/05 12:37:55 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	is_number_str(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] == ' ' || (s[i] >= 9 && *s <= 13 ))
		i++;
	if (!s || s[i] == '\0')
		return (0);
	if (s[i] == '+')
		i++;
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

static long long	ft_atoll_check(const char *s, int *error)
{
	long long	res;
	int			sign;

	sign = 1;
	res = 0;
	while (*s == ' ' || (*s >= 9 && *s <= 13 ))
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s++ == '-')
			sign = -sign;
	}
	while (*s >= '0' && *s <= '9')
	{
		if (res > 9223372036854775807 / 10)
			*error = 1;
		res = res * 10 + (*s - '0');
		s++;
	}
	return (res * sign);
}

static int	fill_scheduler(t_args *args, char *mode)
{
	if (strcmp(mode, "fifo") == 0)
		args->scheduler = SCHEDULER_FIFO;
	else if (strcmp(mode, "edf") == 0)
		args->scheduler = SCHEDULER_EDF;
	else
		return (0);
	return (1);
}

int	validate_args(t_args *args)
{
	long long	limit;

	if (args->number_of_coders < 1 || args->number_of_coders > 2000)
		return (print_error("Number of coders must be between 1 and 2000.\n"));
	if (args->number_of_compiles_required > INT_MAX
		|| args->number_of_compiles_required == 0)
		return (print_error("Number of compiles "
				"must be between 1 and INT_MAX\n"));
	limit = 9223372036854775807LL / 1000;
	if (args->time_to_burnout > limit || args->time_to_compile > limit
		|| args->time_to_debug > limit || args->time_to_refactor > limit
		|| args->dongle_cooldown > limit)
		return (print_error("Time values are too high : overflow risk\n"));
	if (args->time_to_burnout <= 0 || args->time_to_compile <= 0
		|| args->time_to_debug <= 0 || args->time_to_refactor <= 0)
		return (print_error("Negative or zero invalid value"));
	if (args->dongle_cooldown < 0)
		return (print_error("Dongle_cooldown can not be a negative value."));
	return (1);
}

int	fill_args_structure(t_args *args, int argc, char **argv)
{
	size_t		i;
	int			error;
	long long	*ptr;

	i = 0;
	error = 0;
	if (argc != 9)
		return (print_error("Required : number_of_coders, time_to_burnout, "
				"time_to_compile, time_to_debug, time_to_refactor, "
				"number_of_compiles_required, dongle_cooldown, scheduler"));
	while (i++ < 7)
	{
		if (!is_number_str(argv[i]))
			return (print_error("Args 1 to 7 must be positive integers."));
	}
	args->number_of_coders = (size_t)ft_atoll_check(argv[1], &error);
	args->number_of_compiles_required = (size_t)ft_atoll_check(argv[6], &error);
	ptr = &args->time_to_burnout;
	i = 2;
	while (i <= 5)
		*(ptr++) = (long long)ft_atoll_check(argv[i++], &error);
	args->dongle_cooldown = (long long)ft_atoll_check(argv[7], &error);
	if (error == 1 || !fill_scheduler(args, argv[8]))
		return (print_error("Scheduler must be exactly 'fifo' or 'edf'"));
	return (validate_args(args));
}
