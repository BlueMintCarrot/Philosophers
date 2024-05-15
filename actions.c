/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joanda-s <joanda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 15:55:26 by joanda-s          #+#    #+#             */
/*   Updated: 2024/05/15 17:18:36 by joanda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	athena(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->lock);
	if (philo->info->still_alive == 0)
	{
		pthread_mutex_unlock(&philo->info->lock);
		return ;
	}
	pthread_mutex_unlock(&philo->info->lock);
	leave_a_message_after_the_beep(philo, 4);
	pthread_mutex_lock(&philo->info->lock);
	if (philo->info->nbr_of_philo % 2 == 0)
		philo->info->time_to_think = philo->info->time_to_eat
			- philo->info->time_to_sleep;
	else if (philo->info->nbr_of_philo % 2 == 1)
	{
		philo->info->time_to_think = ((philo->info->time_to_eat * 2)
				- philo->info->time_to_sleep) * 0.42;
	}
	pthread_mutex_unlock(&philo->info->lock);
	pthread_mutex_lock(&philo->info->lock);
	hypnos(tva() + philo->info->time_to_think);
	pthread_mutex_unlock(&philo->info->lock);
	dionysus(philo);
}

void	morpheus(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->lock);
	if (philo->info->still_alive == 0)
	{
		pthread_mutex_unlock(&philo->info->lock);
		return ;
	}
	pthread_mutex_unlock(&philo->info->lock);
	leave_a_message_after_the_beep(philo, 3);
	hypnos(tva() + philo->info->time_to_sleep);
	athena(philo);
}

void	dionysus_helper(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->lock);
	philo->info->nbr_of_full_philo++;
	pthread_mutex_unlock(&philo->info->lock);
	pthread_mutex_unlock(&philo->chronos);
}

void	dionysus(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->lock);
	if (philo->info->still_alive == 0)
	{
		pthread_mutex_unlock(&philo->info->lock);
		return ;
	}
	pthread_mutex_unlock(&philo->info->lock);
	pick_up_forks(philo);
	pthread_mutex_lock(&philo->chronos);
	philo->time_til_death = tva() + philo->info->time_til_death;
	pthread_mutex_unlock(&philo->chronos);
	leave_a_message_after_the_beep(philo, 2);
	philo->nbr_of_meals--;
	hypnos(tva() + philo->info->time_to_eat);
	put_down_forks(philo);
	pthread_mutex_lock(&philo->chronos);
	if (philo->nbr_of_meals == 0)
	{
		dionysus_helper(philo);
		return ;
	}
	pthread_mutex_unlock(&philo->chronos);
	morpheus(philo);
}

void	hypnos(long int time_limit)
{
	while (tva() < time_limit)
		usleep(300);
}
