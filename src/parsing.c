/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: stmaire <stmaire@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/30 11:56:02 by stephanie         #+#    #+#             */
/*   Updated: 2026/05/04 17:55:17 by stmaire          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	validate_args(t_args *args)
{
	if (args->number_of_coders <= 0)
		return (0);
	if (args->time_to_burnout <= 0 || args->time_to_compile < 0)
		return (0);
	if (args->time_to_debug < 0 || args->time_to_refactor < 0)
		return (0);
	if (args->number_of_compiles_required < 0 || args-> dongle_cooldown < 0)
		return (0);
	if (args->scheduler != SCHEDULER_FIFO && args->scheduler != SCHEDULER_EDF)
		return (0);
	return (1);

}

int	fill_args_structure(t_args *args, int argc,char **argv)
{
	int	i;
	int error;
	long long *ptr;

	i = 0;
	error = 0;
	if (argc != 9)
		return (0);
	while (i++ < 7)
	{
		if (!is_number_str(argv[i]))
			return (0);
	}
	args->number_of_coders = (size_t)ft_atoll_check(argv[1], &error);
	args->number_of_compiles_required = (size_t)ft_atoll_check(argv[6], &error);
	ptr = &args->time_to_burnout;
	i = 2;
	while (i <= 5)
		*(ptr++) = (long long)ft_atoll_check(argv[i++], &error);
	args->dongle_cooldown = (long long)ft_atoll_check(argv[7], &error);
	if (error == 1 || !fill_scheduler(args, argv[8]))
		return (0);
	return (validate_args(args));
}

int fill_scheduler(t_args *args, char *mode)
{
    if (ft_strcmp(mode, "fifo") == 0)
        args->scheduler = SCHEDULER_FIFO;
    else if (ft_strcmp(mode, "edf") == 0)
        args->scheduler = SCHEDULER_EDF;
    else
        return (0);
    return (1);
}

static int	is_number_str(char *s)
{
	size_t	i;

	i = 0;
	if (!s || s[i] == '\0')
		return (0);
	while (s[i])
	{
		if (s[i] < '0' || 	s[i] > '9')
			return (0);
		i++;
	}
	return (1);
}
static long long ft_atoll_check(const char *s, int *error)
{
	long long res;
	int	sign;

	sign = 1;
	*error = 0;
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
	if (res < 0) // en cas d'overflow, le signe peut basculer
		*error = 1;
	return (res * sign);
}



