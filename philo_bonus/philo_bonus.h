/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 13:10:45 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/20 13:10:45 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <semaphore.h>
# include <limits.h>
# include <stdio.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/time.h>
# include <sys/wait.h>
# include <pthread.h>

# define SEM_FORKS "/philo_forks"
# define SEM_PRINT "/philo_print"
# define SEM_DEAD "/philo_dead"
# define SEM_UPDATE "/philo_update"

typedef struct s_data
{
	int				n;
	unsigned long	time_to_eat;
	unsigned long	time_to_sleep;
	unsigned long	time_to_die;
	unsigned long	eat_max;
	pthread_t		watcher;
	int				count_eat;
	sem_t			*forks;
	sem_t			*printing;
	sem_t			*dead;
	sem_t			*update;
	int				pid[200];
}					t_data;

typedef struct philo
{
	int				id;
	t_data			*data;
	unsigned long	eat_count;
	unsigned long	last_eat;
	pthread_t		watcher;
	pthread_t		time_to_die;
}					t_philo;

int				parse_args(int ac, char **av, t_data *data);
int				philo_process(t_data *data, int id);
void			ft_sleep(unsigned long mili_sec);
unsigned long	get_timestamp(void);
void			*watcher(void *arg);

#endif // !PHILO_BONUS_H
