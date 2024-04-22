/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 16:50:25 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/20 16:50:25 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*watcher(void *arg)
{
	t_philo			*philo;
	unsigned long	last_eat;
	unsigned long	eat_count;

	philo = arg;
	while (1)
	{
		ft_sleep(5);
		sem_wait(philo->data->update);
		last_eat = philo->last_eat;
		eat_count = philo->eat_count;
		sem_post(philo->data->update);
		if (philo->data->count_eat && eat_count >= philo->data->eat_max)
			return (NULL);
		if (get_timestamp() - last_eat >= philo->data->time_to_die)
			break ;
	}
	sem_wait(philo->data->printing);
	printf("%ld %i died\n", get_timestamp(), philo->id);
	sem_post(philo->data->dead);
	return (NULL);
}

void	eat(t_philo *philo)
{
	sem_wait(philo->data->forks);
	sem_wait(philo->data->printing);
	printf("%ld %i has taken a fork\n", get_timestamp(), philo->id);
	sem_post(philo->data->printing);
	sem_wait(philo->data->forks);
	sem_wait(philo->data->printing);
	printf("%ld %i has taken a fork\n", get_timestamp(), philo->id);
	printf("%ld %i is eating\n", get_timestamp(), philo->id);
	sem_post(philo->data->printing);
	sem_wait(philo->data->update);
	philo->last_eat = get_timestamp();
	sem_post(philo->data->update);
	ft_sleep(philo->data->time_to_eat);
	sem_post(philo->data->forks);
	sem_post(philo->data->forks);
}

void	routine(t_philo *philo)
{
	sem_wait(philo->data->printing);
	printf("%ld %i is thinking\n", get_timestamp(), philo->id);
	sem_post(philo->data->printing);
	if (philo->data->n % 2)
		ft_sleep(philo->data->time_to_eat * (philo->id - 1) / philo->data->n);
	else if (philo->id % 2)
		ft_sleep(philo->data->time_to_eat / 2);
	while (1)
	{
		eat(philo);
		sem_wait(philo->data->update);
		if (philo->data->count_eat
			&& philo->eat_count++ == philo->data->eat_max)
			return ;
		sem_post(philo->data->update);
		sem_wait(philo->data->printing);
		printf("%ld %i is sleeping\n", get_timestamp(), philo->id);
		sem_post(philo->data->printing);
		ft_sleep(philo->data->time_to_sleep);
		sem_wait(philo->data->printing);
		printf("%ld %i is thinking\n", get_timestamp(), philo->id);
		sem_post(philo->data->printing);
	}
}

int	philo_process(t_data *data, int id)
{
	t_philo	philo;

	philo.id = id;
	philo.data = data;
	philo.eat_count = 0;
	philo.last_eat = 0;
	pthread_create(&philo.watcher, NULL, &watcher, &philo);
	routine(&philo);
	sem_post(philo.data->update);
	pthread_join(philo.watcher, NULL);
	sem_close(data->forks);
	sem_close(data->printing);
	sem_close(data->dead);
	sem_close(data->update);
	return (0);
}
