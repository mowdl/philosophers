/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   data_threads.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/23 17:04:52 by mel-meka          #+#    #+#             */
/*   Updated: 2024/03/23 17:04:52 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_timestamp()
{
	static int				init;
	static struct timeval	start_time;
	struct timeval			now;

	if (init == 0)
	{
		gettimeofday(&start_time, NULL);
		init = 1;
	}
	gettimeofday(&now, NULL);
	return (now.tv_sec - start_time.tv_sec) * 1000 + (now.tv_usec - start_time.tv_usec) / 1000;
}

void	ft_sleep(unsigned long mili_sec)
{
	unsigned long	start_time;

	start_time = get_timestamp();
	while ((get_timestamp() - start_time) < mili_sec)
		usleep(1);
}

void	eat(t_philo *philo)
{
	pthread_mutex_t	*first_fork;
	pthread_mutex_t	*second_fork;

	if (philo->id == 1)
	{
		first_fork = philo->left_fork;
		second_fork = philo->right_fork;
	}
	else
	{
		first_fork = philo->right_fork;
		second_fork = philo->left_fork;
	}
	pthread_mutex_lock(first_fork);
	pthread_mutex_lock(&philo->data->printing);
	printf("%ld %i has taken a fork\n", get_timestamp(), philo->id);
	pthread_mutex_unlock(&philo->data->printing);
	pthread_mutex_lock(second_fork);
	pthread_mutex_lock(&philo->data->printing);
	printf("%ld %i has taken a fork\n", get_timestamp(), philo->id);
	printf("%ld %i is eating\n", get_timestamp(), philo->id);
	pthread_mutex_unlock(&philo->data->printing);
	philo->last_eat_time = get_timestamp();
	ft_sleep(philo->data->time_to_eat);
	philo->last_eat_time = get_timestamp();
	pthread_mutex_unlock(first_fork);
	pthread_mutex_unlock(second_fork);
	philo->eat_counter++;
}

void	*philo_routine(void *arg)
{
	t_philo	*self;

	self = (t_philo *)arg;
	while (self->data->dead == 0)
	{
		eat(self);
		pthread_mutex_lock(&self->data->printing);
		printf("%ld %i is sleeping\n", get_timestamp(), self->id);
		pthread_mutex_unlock(&self->data->printing);
		ft_sleep(self->data->time_to_sleep);
		pthread_mutex_lock(&self->data->printing);
		printf("%ld %i is thinking\n", get_timestamp(), self->id);
		pthread_mutex_unlock(&self->data->printing);
	}
	return (0);
}


void	*watcher(void *arg)
{
	int		i;
	t_data	*data;

	data = (t_data *)arg;
	while (data->dead == 0)
	{
		i = 0;
		while (i < data->n)
		{
			if ((get_timestamp() - data->philos[i].last_eat_time) >= data->time_to_die)
			{
				data->dead = 1;
				pthread_mutex_lock(&data->printing);
				printf("%ld %i DIED for %ld\n",
					get_timestamp(),
					(i + 1),
					(get_timestamp() - data->philos[i].last_eat_time));
				printf("last eat time %i\n", data->philos[i].last_eat_time);
				break ;
			}
			i++;
		}
	}
	return (0);
}

int	create_threads(t_data *data)
{
	long i;

	i = 0;
	while (i < data->n)
	{
		if (pthread_create(&data->philos[i].thread,
				NULL,
				&philo_routine,
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
	pthread_join(data->watcher_thread, NULL);
	return (0);
}
