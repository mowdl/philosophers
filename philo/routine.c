/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/25 13:15:51 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/26 11:24:27 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(philo->left_fork);
		return (1);
	}
	printf("%ld %i has taken a fork\n", get_timestamp(), philo->id);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	pthread_mutex_lock(philo->right_fork);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead)
	{
		pthread_mutex_unlock(philo->left_fork);
		pthread_mutex_unlock(philo->right_fork);
		return (1);
	}
	printf("%ld %i has taken a fork\n", get_timestamp(), philo->id);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	return (0);
}

int	eat_and_sleep(t_philo *philo)
{
	if (take_forks(philo))
		return (1);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead)
		return (1);
	philo->last_eat_time = get_timestamp();
	printf("%ld %i is eating\n", get_timestamp(), philo->id);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	ft_sleep(philo->data->time_to_eat);
	pthread_mutex_unlock(philo->left_fork);
	pthread_mutex_unlock(philo->right_fork);
	pthread_mutex_lock(&philo->data->dead_mutex);
	if ((philo->data->count_eat && ++philo->eat_counter == philo->data->eat_max)
		|| philo->data->dead)
		return (1);
	printf("%ld %i is sleeping\n", get_timestamp(), philo->id);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	ft_sleep(philo->data->time_to_sleep);
	return (0);
}

void	*one_philo_case(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	printf("%ld %i has taken a fork\n", get_timestamp(), philo->id);
	ft_sleep(philo->data->time_to_die);
	pthread_mutex_unlock(philo->left_fork);
	return (NULL);
}

void	*philo_routine(t_philo *philo)
{
	unsigned long	last_eat_time;

	pthread_mutex_lock(&philo->data->dead_mutex);
	if (philo->data->dead)
		return (NULL);
	printf("%ld %i is thinking\n", get_timestamp(), philo->id);
	pthread_mutex_unlock(&philo->data->dead_mutex);
	if (philo->data->n == 1)
		return (one_philo_case(philo));
	if (philo->id % 2)
		ft_sleep(philo->data->time_to_eat / 2);
	while (1)
	{
		if (eat_and_sleep(philo))
			break ;
		pthread_mutex_lock(&philo->data->dead_mutex);
		if (philo->data->dead)
			break ;
		printf("%ld %i is thinking\n", get_timestamp(), philo->id);
		last_eat_time = philo->last_eat_time;
		pthread_mutex_unlock(&philo->data->dead_mutex);
		ft_sleep((philo->data->time_to_die
				- (get_timestamp() - last_eat_time)) / 2);
	}
	return (NULL);
}

void	*philo_routine_wraper(void *arg)
{
	philo_routine((t_philo *)arg);
	pthread_mutex_unlock(&((t_philo *)arg)->data->dead_mutex);
	return (NULL);
}
