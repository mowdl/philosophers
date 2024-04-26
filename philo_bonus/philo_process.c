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

unsigned long	get_last_eat_time(t_philo *philo)
{
	unsigned long	tmp;

	sem_wait(philo->data->update);
	tmp = philo->last_eat;
	sem_post(philo->data->update);
	return (tmp);
}

void	philo_sleep(t_philo *philo)
{
	sem_wait(philo->data->printing);
	printf("%ld %i is sleeping\n", get_timestamp(), philo->id);
	sem_post(philo->data->printing);
	ft_sleep(philo->data->time_to_sleep);
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
		philo_sleep(philo);
		sem_wait(philo->data->printing);
		printf("%ld %i is thinking\n", get_timestamp(), philo->id);
		sem_post(philo->data->printing);
		ft_sleep((philo->data->time_to_die
				- (get_timestamp() - get_last_eat_time(philo)))
			/ 2);
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
