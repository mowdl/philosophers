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

	philo = arg;
	while (1)
	{
		sem_wait(philo->data->update);
		last_eat = philo->last_eat;
		sem_post(philo->data->update);
		if (get_timestamp() - last_eat >= philo->data->eat_max)
			break ;
	}
	sem_wait(philo->data->printing);
	printf("%ld %i died\n", get_timestamp(), philo->id);
	sem_post(philo->data->dead);
	return (NULL);
}

void	*time_to_die(void *arg)
{
	t_data *data;
	int sval = -1;

	data = arg;
sem_getvalue(data->forks, &sval);
printf("REACHED %i\n", sval);
	sem_wait(data->dead);
	sem_post(data->dead);
	sem_close(data->forks);
	sem_close(data->printing);
	sem_close(data->dead);
	sem_close(data->update);
	exit(0);
	return (NULL);
}

void	init_philo(t_philo *philo, t_data *data, int id)
{
	philo->id = id;
	philo->data = data;
	philo->eat_count = 0;
	philo->last_eat = 0;
}

void	routine(t_philo *philo)
{
	while (1)
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
		if (philo->data->count_eat && ++philo->eat_count == philo->data->eat_max)
			break ;
		sem_wait(philo->data->printing);
		printf("%ld %i is sleeping\n", get_timestamp(), philo->id);
		sem_post(philo->data->printing);
		ft_sleep(philo->data->time_to_sleep);
		sem_wait(philo->data->printing);
		printf("%ld %i is thinking\n", get_timestamp(), philo->id);
		sem_post(philo->data->printing);
	}
}

void	philo_process(t_data *data, int id)
{
	t_philo	philo;

	init_philo(&philo, data, id);
	pthread_create(&philo.watcher, NULL, &watcher, &philo);
	pthread_create(&philo.time_to_die, NULL, &time_to_die, &philo);

	routine(&philo);

	sem_close(data->forks);
	sem_close(data->printing);
	sem_close(data->dead);
	sem_close(data->update);

	exit(0);
}
