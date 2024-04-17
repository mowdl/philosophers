/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:59:02 by mel-meka          #+#    #+#             */
/*   Updated: 2024/03/22 19:59:02 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "philo.h"

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

int	philo_clean(t_data *data)
{
	free(data->philos);
	free(data->forks);
	return (0);
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
		i++;
	}
	return (0);
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
	i = 0;
	while (i < data->n)
	{
		data->philos[i].left_fork = &data->forks[(i + 1) % data->n];
		data->philos[i].right_fork = &data->forks[i % data->n];
		i++;
	}
	return (0);
}

int	parse_int(char *str, unsigned long *n)
{
	size_t	i;

	i = 0;
	if (str[i] == '\0')
		return (1);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (1);
		i++;
	}
	i = 0;
	*n = 0;
	while (str[i])
	{
		*n = *n * 10 + (str[i] - '0');
		if (*n > INT_MAX)
			return (1);
		i++;
	}
	printf("Parsed %ld\n", *n);
	return (0);
}

int	parse_args(int ac, char **av, t_data *data)
{
	unsigned long	tmp;
	if (ac != 5 && ac != 6)
		return (1);
	if (parse_int(av[1], &tmp))
		return (1);
	data->n = (int)tmp;
	if (parse_int(av[2], &data->time_to_die))
		return (1);
	if (parse_int(av[3], &data->time_to_eat))
		return (1);
	if (parse_int(av[4], &data->time_to_sleep))
		return (1);
	if (ac == 6)
	{
		data->count_eat = 1;
		if (parse_int(av[5], &data->eat_max))
			return (1);
	}
	else
		data->count_eat = 0;
	return (0);
}

int	main(int ac, char **av)
{
	t_data	data;
	if (parse_args(ac, av, &data))
		return (1);
	data.dead = 0;
	if (init_philo(&data))
		return (1);
	if (init_forks(&data))
		return (1);
	if (pthread_mutex_init(&data.printing, NULL))
		return (1);
	if (create_threads(&data))
		return (1);
	join_threads(&data);
}
