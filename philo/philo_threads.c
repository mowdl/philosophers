/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:04:52 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/25 14:34:55 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philo_routine_wraper(void *arg)
{
	return (philo_routine((t_philo *)arg));
}

int	create_threads(t_data *data)
{
	long	i;

	get_timestamp();
	i = 0;
	while (i < data->n)
	{
		if (pthread_create(&data->philos[i].thread,
				NULL,
				&philo_routine_wraper,
				(void *)&data->philos[i]))
			return (1);
		i++;
	}
	if (pthread_create(&data->watcher_thread, NULL, &watcher, data))
		return (1);
	return (0);
}

int	join_threads(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n)
	{
		pthread_join(data->philos[i].thread, NULL);
		i++;
	}
	pthread_join(data->watcher_thread, NULL);
	return (0);
}
