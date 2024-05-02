/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:59:02 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/26 08:52:02 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	init_philo(t_data *data)
{
	int	i;

	memset(data->philos, 0, sizeof(t_philo) * data->n);
	i = 0;
	while (i < data->n)
	{
		data->philos[i].id = i + 1;
		data->philos[i].last_eat_time = 0;
		data->philos[i].data = data;
		data->philos[i].eat_counter = 0;
		i++;
	}
	return (0);
}

void	assign_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n)
	{
		if (i % 2)
		{
			data->philos[i].left_fork = &data->forks[(i + 1) % data->n];
			data->philos[i].right_fork = &data->forks[i];
		}
		else
		{
			data->philos[i].right_fork = &data->forks[(i + 1) % data->n];
			data->philos[i].left_fork = &data->forks[i];
		}
		i++;
	}
}

int	init_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n)
	{
		if (pthread_mutex_init(data->forks + i++, NULL))
			return (1);
	}
	assign_forks(data);
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;

	if (parse_args(ac, av, &data))
		return (1);
	if (data.count_eat && data.eat_max == 0)
		return (0);
	if (pthread_mutex_init(&data.dead_mutex, NULL))
		return (1);
	data.dead = 0;
	if (init_philo(&data))
		return (1);
	if (init_forks(&data))
		return (1);
	if (create_threads(&data))
		return (1);
	join_threads(&data);
}
