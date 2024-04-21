/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:01:31 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/20 11:01:31 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

int	spawn_philos(t_data *data)
{
	int	i;

	get_timestamp();
	i = 0;
	while (i < data->n)
	{
		data->pid[i] = fork();
		if (data->pid[i] == -1)
			return (1);
		if (data->pid[i] == 0)
			philo_process(data, i + 1);
		i++;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;
	int		i;
	if (parse_args(ac, av, &data))
		return (1);



	data.forks = sem_open(SEM_FORKS, O_CREAT | O_EXCL, 00666, data.n);
	data.printing = sem_open(SEM_PRINT, O_CREAT | O_EXCL, 00666, 1);
	data.dead = sem_open(SEM_DEAD, O_CREAT | O_EXCL, 00666, 0);
	data.update = sem_open(SEM_UPDATE, O_CREAT | O_EXCL, 00666, 1);
	if (data.forks == NULL || data.printing == NULL
		|| data.dead == NULL || data.update == NULL)
	{
		printf("sem already exists\n");
		sem_unlink(SEM_FORKS);
		sem_unlink(SEM_PRINT);
		sem_unlink(SEM_DEAD);
		sem_unlink(SEM_UPDATE);
		return (1);
	}
	


	spawn_philos(&data);
	
	i = 0;
	while (i < data.n)
		waitpid(data.pid[i++], NULL, 0);

	sem_close(data.forks);
	sem_close(data.printing);
	sem_close(data.dead);
	sem_close(data.update);
	sem_unlink(SEM_FORKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_DEAD);
	sem_unlink(SEM_UPDATE);
}
