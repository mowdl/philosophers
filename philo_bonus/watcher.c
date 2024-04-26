/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   watcher.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:31:30 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/26 15:31:30 by mel-meka         ###   ########.fr       */
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
