/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 14:31:56 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/25 15:01:00 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	is_done(t_data *data, int i)
{
	if (data->count_eat && data->philos[i].eat_counter == data->eat_max)
		return (1);
	return (0);
}

void	unlock_forks(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->n)
	{
		pthread_mutex_unlock(&data->forks[i]);
		i++;
	}
}

int	watcher_loop(t_data *data, int *done_flag)
{
	int	i;

	i = 0;
	while (i < data->n)
	{
		pthread_mutex_lock(&data->dead_mutex);
		if (!is_done(data, i))
		{
			*done_flag = 0;
			if ((get_timestamp() - data->philos[i].last_eat_time)
				>= data->time_to_die)
			{
				printf("%ld %i died\n", get_timestamp(), i + 1);
				data->dead = 1;
				pthread_mutex_unlock(&data->dead_mutex);
				unlock_forks(data);
				return (1);
			}
		}
		pthread_mutex_unlock(&data->dead_mutex);
		i++;
	}
	return (0);
}

void	*watcher(void *arg)
{
	t_data	*data;
	int		done_flag;

	data = arg;
	done_flag = 0;
	while (!done_flag)
	{
		done_flag = 1;
		if (watcher_loop(data, &done_flag))
			return (NULL);
	}
	return (NULL);
}
