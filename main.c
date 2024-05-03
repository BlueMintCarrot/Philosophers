/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joanda-s <joanda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:01:46 by joanda-s          #+#    #+#             */
/*   Updated: 2024/05/03 20:59:44 by joanda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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
	return (z * y);
}

t_info	*info_init(int argc, char **argv)
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
	info->nbr_of_meals = ft_llato(argv[5]);
	pthread_mutex_init(&info->lock, NULL);
	pthread_mutex_init(&info->beep, NULL);
	gettimeofday(&time, NULL);
	info->start_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	info->fork = malloc(sizeof(pthread_mutex_t) * info->nbr_of_philo);
	while (n < info->nbr_of_philo)
	{
		pthread_mutex_init(&info->fork[n], NULL);
		n++;
	}
}

t_philo	*philo_init(t_info *info)
{
	t_philo	*philo;
	int	n;

	philo = malloc(sizeof(t_philo) * info->nbr_of_philo);
	n = 0;
	while (n < info->nbr_of_philo)
	{
		philo[n].info = info;
		philo[n].time_til_death = info->time_til_death;
		philo[n].seat_pos = n + 1;
		n++;
	}
}

void	*eat_sleep_repeat(void *philo)
{
	t_philo	*philo;

	philo = (t_philo *)philo;
	pick_up_forks(philo);
	
	put_down_forks(philo);
	return (NULL);
}

void	pick_up_forks(t_philo *philo)
{
	if (philo->seat_pos % 2 == 0)
	{
		if (philo->seat_pos == philo->info->nbr_of_philo)
			pthread_mutex_lock(&philo->info->fork[0]);
		else
			pthread_mutex_lock(&philo->info->fork[philo->seat_pos]);
		leave_a_message_after_the_beep(philo);
		pthread_mutex_lock(&philo->info->fork[philo->seat_pos - 1]);
		leave_a_message_after_the_beep(philo);
	}
	else if (philo->seat_pos % 2 == 1)
	{
		pthread_mutex_lock(&philo->info->fork[philo->seat_pos - 1]);
		leave_a_message_after_the_beep(philo);
		if (philo->seat_pos == philo->info->nbr_of_philo)
			pthread_mutex_lock(&philo->info->fork[0]);
		else
			pthread_mutex_lock(&philo->info->fork[philo->seat_pos]);
		leave_a_message_after_the_beep(philo);
	}
}

long	tva(void)
{
	struct timeval	time;
	long int		sacred_time;

	gettimeofday(&time, NULL);
	sacred_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (sacred_time);
}

void	leave_a_message_after_the_beep(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->beep);
}

void	put_down_forks(t_philo *philo)
{
	if (philo->seat_pos % 2 == 0)
	{
		if (philo->seat_pos == philo->info->nbr_of_philo)
			pthread_mutex_unlock(&philo->info->fork[0]);
		else
			pthread_mutex_unlock(&philo->info->fork[philo->seat_pos]);
		leave_a_message_after_the_beep(philo);
		pthread_mutex_unlock(&philo->info->fork[philo->seat_pos - 1]);
		leave_a_message_after_the_beep(philo);
	}
	else if (philo->seat_pos % 2 == 1)
	{
		pthread_mutex_unlock(&philo->info->fork[philo->seat_pos - 1]);
		leave_a_message_after_the_beep(philo);
		if (philo->seat_pos == philo->info->nbr_of_philo)
			pthread_mutex_unlock(&philo->info->fork[0]);
		else
			pthread_mutex_unlock(&philo->info->fork[philo->seat_pos]);
		leave_a_message_after_the_beep(philo);
	}
}

int main(int argc, char **argv)
{
	t_info	*info;
	t_philo	*philo;
	int	n;

	n = 0;
	info = info_init(argc, argv);
	philo = philo_init(info);
	while (n < info->nbr_of_philo)
	{
		pthread_create(&philo[n].id, NULL, eat_sleep_repeat, &philo[n]);
		n++;
	}
	n = 0;
	while (n < info->nbr_of_philo)
	{
		pthread_join(philo[n].id, NULL);
		n++;
	}
	return (0);
}