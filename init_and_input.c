/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file1.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joanda-s <joanda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/15 15:55:24 by joanda-s          #+#    #+#             */
/*   Updated: 2024/05/15 16:04:04 by joanda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
#define M "How would you run a simulation without any philosophers??\n"

long long	ft_llato(const char *str)
{
	int			x;
	int			y;
	long int	z;

	x = 0;
	y = 1;
	z = 0;
	while ((str[x] >= 9 && str[x] <= 13) || (str[x] == 32))
		x++;
	if (str[x] == '-' || str[x] == '+')
	{
		if (str[x] == '-')
			y = -1;
		x++;
	}
	while (str[x] >= 48 && str[x] <= 57)
	{
		z = (z * 10) + (str[x] - 48);
		x++;
	}
	if ((str[x] < 48 || str[x] > 57) && str[x] != '\0')
		return (0);
	return (z * y);
}

int	input_verifications(t_philo *philo, int argc)
{
	if (philo->info->nbr_of_philo < 1)
	{
		write(1, M, 58);
		return (0);
	}
	if (philo->info->time_til_death < 1 || philo->info->time_to_eat < 1
		|| philo->info->time_to_sleep < 1)
	{
		write(1, "Invalid times to perform actions!\n", 34);
		return (0);
	}
	if (philo->nbr_of_meals <= 0 && argc == 6)
	{
		write(1, "No meals available, they will starve!\n", 38);
		return (0);
	}
	return (1);
}

t_info	*info_init(char **argv)
{
	int				n;
	t_info			*info;
	struct timeval	time;

	n = 0;
	info = malloc(sizeof(t_info));
	info->nbr_of_philo = ft_llato(argv[1]);
	info->time_til_death = ft_llato(argv[2]);
	info->time_to_eat = ft_llato(argv[3]);
	info->time_to_sleep = ft_llato(argv[4]);
	info->nbr_of_full_philo = 0;
	info->still_alive = 1;
	pthread_mutex_init(&info->lock, NULL);
	pthread_mutex_init(&info->beep, NULL);
	gettimeofday(&time, NULL);
	info->start_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	info->fork = malloc(sizeof(pthread_mutex_t) * info->nbr_of_philo);
	if (!info->fork)
		return (NULL);
	while (n < info->nbr_of_philo)
	{
		pthread_mutex_init(&info->fork[n], NULL);
		n++;
	}
	return (info);
}

t_philo	*philo_init(t_info *info, char **argv)
{
	t_philo	*philo;
	int		n;

	if (info->nbr_of_philo <= 0)
		return (NULL);
	philo = malloc(sizeof(t_philo) * info->nbr_of_philo);
	if (!philo)
		return (NULL);
	n = 0;
	while (n < info->nbr_of_philo)
	{
		philo[n].info = info;
		philo[n].time_til_death = info->start_time + info->time_til_death;
		if (argv[5])
			philo[n].nbr_of_meals = ft_llato(argv[5]);
		else
			philo[n].nbr_of_meals = -1;
		philo[n].seat_pos = n + 1;
		pthread_mutex_init(&philo[n].chronos, NULL);
		n++;
	}
	return (philo);
}

void	philo_cleaner(t_philo *philo)
{
	int	n;

	n = 0;
	pthread_mutex_destroy(&philo->info->beep);
	while (n < philo->info->nbr_of_philo)
	{
		pthread_mutex_destroy(&philo->info->fork[n]);
		pthread_mutex_destroy(&philo[n].chronos);
		n++;
	}
	pthread_mutex_destroy(&philo->info->lock);
	free(philo->info->fork);
	free(philo->info);
	free(philo);
}
