/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 19:59:10 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/26 08:51:54 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>
# include <limits.h>

struct			s_data;

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	int				id;
	unsigned long	eat_counter;
	unsigned long	last_eat_time;
	struct s_data	*data;
}					t_philo;

typedef struct s_data
{
	t_philo			*philos;
	pthread_t		watcher_thread;
	pthread_mutex_t	*forks;
	int				n;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	time_to_die;
	unsigned long	eat_max;
	int				count_eat;
	int				dead;
	pthread_mutex_t	dead_mutex;
}					t_data;

int				parse_args(int ac, char **av, t_data *data);
int				create_threads(t_data *data);
int				join_threads(t_data *data);

void			*watcher(void *arg);

void			*philo_routine(t_philo *philo);
void			ft_sleep(unsigned long mili_sec);

unsigned long	get_timestamp(void);

#endif // !PHILO_H
