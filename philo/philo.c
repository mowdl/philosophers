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

int	philo_clean(t_data *data)
{
	free(data->philos);
	free(data->forks);
	return (1);
}

int	init_philo(t_data *data)
{
	int	i;

	data->philos = malloc(sizeof(t_philo) * data->n);
	if (data->philos == NULL)
		return (-1);
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

	data->forks = malloc(sizeof(pthread_mutex_t) * data->n);
	if (data->forks == NULL)
		return (1);
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
		return (philo_clean(&data));
	if (data.count_eat && data.eat_max == 0)
		return (0);
	if (pthread_mutex_init(&data.dead_mutex, NULL))
		return (philo_clean(&data));
	data.dead = 0;
	if (init_philo(&data))
		return (philo_clean(&data));
	if (init_forks(&data))
		return (philo_clean(&data));
	if (create_threads(&data))
		return (philo_clean(&data));
	join_threads(&data);
	philo_clean(&data);
}
