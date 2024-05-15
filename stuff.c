/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stuff.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joanda-s <joanda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 15:55:28 by joanda-s          #+#    #+#             */
/*   Updated: 2024/05/15 17:18:56 by joanda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	are_you_still_there_helper(t_philo *philo, int n)
{
	long int	sacred_time;

	pthread_mutex_lock(&philo->info->lock);
	philo->info->still_alive = 0;
	pthread_mutex_unlock(&philo->info->lock);
	pthread_mutex_lock(&philo->info->beep);
	sacred_time = tva() - philo->info->start_time;
	ft_putnbr(sacred_time);
	write(1, " ", 1);
	ft_putnbr(n + 1);
	write(1, " died\n", 6);
	pthread_mutex_unlock(&philo->info->beep);
}

void	are_you_still_there(t_philo *philo, int n)
{
	while (1)
	{
		pthread_mutex_lock(&philo[n].chronos);
		if (tva() > philo[n].time_til_death && philo[n].nbr_of_meals != 0)
		{
			pthread_mutex_unlock(&philo[n].chronos);
			break ;
		}
		pthread_mutex_lock(&philo->info->lock);
		if (philo->info->nbr_of_full_philo == philo->info->nbr_of_philo)
		{
			philo->info->still_alive = 0;
			pthread_mutex_unlock(&philo->info->lock);
			pthread_mutex_unlock(&philo[n].chronos);
			return ;
		}
		pthread_mutex_unlock(&philo->info->lock);
		pthread_mutex_unlock(&philo[n].chronos);
		n++;
		if (n == philo->info->nbr_of_philo)
			n = 0;
		usleep(300000);
	}
	are_you_still_there_helper(philo, n);
}

void	pick_up_forks(t_philo *philo)
{
	if (philo->seat_pos % 2 == 0)
	{
		if (philo->seat_pos == philo->info->nbr_of_philo)
			pthread_mutex_lock(&philo->info->fork[0]);
		else
			pthread_mutex_lock(&philo->info->fork[philo->seat_pos]);
		leave_a_message_after_the_beep(philo, 1);
		pthread_mutex_lock(&philo->info->fork[philo->seat_pos - 1]);
		leave_a_message_after_the_beep(philo, 1);
	}
	else if (philo->seat_pos % 2 == 1)
	{
		pthread_mutex_lock(&philo->info->fork[philo->seat_pos - 1]);
		leave_a_message_after_the_beep(philo, 1);
		if (philo->seat_pos == philo->info->nbr_of_philo)
			pthread_mutex_lock(&philo->info->fork[0]);
		else
			pthread_mutex_lock(&philo->info->fork[philo->seat_pos]);
		leave_a_message_after_the_beep(philo, 1);
	}
}

void	put_down_forks(t_philo *philo)
{
	if (philo->seat_pos % 2 == 0)
	{
		if (philo->seat_pos == philo->info->nbr_of_philo)
			pthread_mutex_unlock(&philo->info->fork[0]);
		else
			pthread_mutex_unlock(&philo->info->fork[philo->seat_pos]);
		pthread_mutex_unlock(&philo->info->fork[philo->seat_pos - 1]);
	}
	else if (philo->seat_pos % 2 == 1)
	{
		pthread_mutex_unlock(&philo->info->fork[philo->seat_pos - 1]);
		if (philo->seat_pos == philo->info->nbr_of_philo)
			pthread_mutex_unlock(&philo->info->fork[0]);
		else
			pthread_mutex_unlock(&philo->info->fork[philo->seat_pos]);
	}
}

long int	tva(void)
{
	struct timeval	time;
	long int		sacred_time;

	gettimeofday(&time, NULL);
	sacred_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (sacred_time);
}
