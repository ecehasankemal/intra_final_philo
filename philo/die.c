/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   die.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hece <hece@student.42kocaeli.com.tr>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/16 23:36:04 by hece              #+#    #+#             */
/*   Updated: 2023/04/17 00:00:47 by hece             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int
	must_eating_check(t_table *table)
{
	register int	i;

	if (table->number_of_must_eat < 0)
		return (0);
	i = 0;
	while (i < table->number_of_philo)
	{
		pthread_mutex_lock(&table->count_eat);
		if (table->philos[i++].eat_count < table->number_of_must_eat)
		{
			pthread_mutex_unlock(&table->count_eat);
			return (0);
		}
		else
			pthread_mutex_unlock(&table->count_eat);
	}
	return (1);
}

static inline void
	die_check_while_loop(t_table *table, int index)
{
	while (table->number_of_philo > 1)
	{
		index = -1;
		while (++index < table->number_of_philo)
		{
			pthread_mutex_lock(&table->eat_last);
			pthread_mutex_lock(&table->is_die);
			if ((current_time() - table->philos[index].last_eat)
				>= table->time_to_die)
			{
				print(table->philos + index, DIE);
				pthread_mutex_lock(&table->stop_count);
				table->stop = 1;
				pthread_mutex_unlock(&table->stop_count);
				pthread_mutex_unlock(&table->eat_last);
				return ;
			}
			else
				pthread_mutex_unlock(&table->eat_last);
			pthread_mutex_unlock(&table->is_die);
		}
		if (must_eating_check(table))
			break ;
	}
}

void
	die_check(t_table *table)
{
	register int	index;

	index = 0;
	if (table->number_of_philo == 1)
		print(table->philos, DIE);
	die_check_while_loop(table, index);
	pthread_mutex_lock(&table->stop_count);
	table->stop = 1;
	pthread_mutex_unlock(&table->stop_count);
}
