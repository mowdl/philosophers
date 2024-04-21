/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 13:14:40 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/20 13:14:40 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	parse_int(char *str, unsigned long *n)
{
	size_t	i;

	i = 0;
	if (str[i] == '\0')
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	i = 0;
	*n = 0;
	while (str[i])
	{
		*n = *n * 10 + (str[i] - '0');
		if (*n > INT_MAX)
			return (1);
		i++;
	}
	return (0);
}

int	parse_args(int ac, char **av, t_data *data)
{
	unsigned long	tmp;
	if (ac != 5 && ac != 6)
		return (1);
	if (parse_int(av[1], &tmp))
		return (1);
	data->n = (int)tmp;
	if (parse_int(av[2], &data->time_to_die))
		return (1);
	if (parse_int(av[3], &data->time_to_eat))
		return (1);
	if (parse_int(av[4], &data->time_to_sleep))
		return (1);
	if (ac == 6)
	{
		data->count_eat = 1;
		if (parse_int(av[5], &data->eat_max))
			return (1);
	}
	else
		data->count_eat = 0;
	return (0);
}
