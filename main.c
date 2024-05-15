/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joanda-s <joanda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:01:46 by joanda-s          #+#    #+#             */
/*   Updated: 2024/05/15 16:43:43 by joanda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#define M "How would you run a simulation without any philosophers??\n"

//1 - pick up fork;
//2 - is eating;
//3 - is sleeping;
//4 - is thinking;
//5 - has falecido;

void	*eat_sleep_repeat(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	if (philo->info->nbr_of_philo == 1)
	{
		leave_a_message_after_the_beep(philo, 1);
		hypnos(tva() + philo->info->time_to_eat);
		return (NULL);
	}
	if (philo->seat_pos % 2 != 0)
		hypnos(tva() + philo->info->time_to_eat);
	dionysus(philo);
	return (NULL);
}

void	leave_a_message_after_the_beep_helper(int i)
{
	if (i == 1)
		write(1, "has taken a fork\n", 17);
	else if (i == 2)
		write(1, "is eating\n", 10);
	else if (i == 3)
		write (1, "is sleeping\n", 12);
	else if (i == 4)
		write (1, "is thinking\n", 12);
	else if (i == 5)
		write (1, "died\n", 5);
}

void	leave_a_message_after_the_beep(t_philo *philo, int i)
{
	long int	sacred_time;

	if (i == 0)
	{
		write(1, M, 58);
		return ;
	}
	pthread_mutex_lock(&philo->info->beep);
	pthread_mutex_lock(&philo->info->lock);
	if (philo->info->still_alive == 0)
	{
		pthread_mutex_unlock(&philo->info->lock);
		pthread_mutex_unlock(&philo->info->beep);
		return ;
	}
	pthread_mutex_unlock(&philo->info->lock);
	sacred_time = tva() - philo->info->start_time;
	ft_putnbr(sacred_time);
	write(1, " ", 1);
	ft_putnbr(philo->seat_pos);
	write(1, " ", 1);
	leave_a_message_after_the_beep_helper(i);
	pthread_mutex_unlock(&philo->info->beep);
}

int	main_helper(t_philo *philo, t_info *info, int argc)
{
	if (!info || !philo)
	{
		leave_a_message_after_the_beep(philo, 0);
		return (0);
	}
	if (input_verifications(philo, argc) == 0)
	{
		philo_cleaner(philo);
		return (0);
	}
	return (1);
}

int	main(int argc, char **argv)
{
	t_info	*info;
	t_philo	*philo;
	int		n;

	n = -1;
	if (argc < 5 || argc > 6)
	{
		if (argc < 5)
			write(1, "Not enough arguments!\n", 22);
		else
			write(1, "Too many arguments!\n", 20);
		return (0);
	}
	info = info_init(argv);
	philo = philo_init(info, argv);
	if (main_helper(philo, info, argc) == 0)
		return (0);
	while (++n < info->nbr_of_philo)
		pthread_create(&philo[n].id, NULL, eat_sleep_repeat, &philo[n]);
	are_you_still_there(philo, 0);
	n = -1;
	while (++n < info->nbr_of_philo)
		pthread_join(philo[n].id, NULL);
	philo_cleaner(philo);
	return (0);
}
