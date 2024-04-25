/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-meka <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 05:51:59 by mel-meka          #+#    #+#             */
/*   Updated: 2024/04/23 05:51:59 by mel-meka         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

unsigned long	get_timestamp(void)
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
	return ((now.tv_sec - start_time.tv_sec) * 1000
		+ (now.tv_usec - start_time.tv_usec) / 1000);
}

void	ft_sleep(unsigned long mili_sec)
{
	unsigned long	start_time;

	start_time = get_timestamp();
	while ((get_timestamp() - start_time) < mili_sec)
		usleep(1);
}
