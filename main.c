/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joana <joana@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 16:01:46 by joanda-s          #+#    #+#             */
/*   Updated: 2024/05/15 00:59:58 by joana            ###   ########.fr       */
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
	while (n < info->nbr_of_philo)
	{
		pthread_mutex_init(&info->fork[n], NULL);
		n++;
	}
	return (info);
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

void	are_you_still_there(t_philo *philo)
{
	int	n;

	n = 0;
	while (1)
	{
		pthread_mutex_lock(&philo[n].chronos);
		if (tva() > philo[n].time_til_death && philo[n].nbr_of_meals != 0)
		{
			pthread_mutex_unlock(&philo[n].chronos);
			break ;
		}
		pthread_mutex_unlock(&philo[n].chronos);
		if (philo->info->nbr_of_full_philo == philo->info->nbr_of_philo)
		{
			pthread_mutex_lock(&philo->info->lock);
			philo->info->still_alive = 0;
			pthread_mutex_unlock(&philo->info->lock);
			return ;
		}
		n++;
		if (n == philo->info->nbr_of_philo)
			n = 0;
	}
	are_you_still_there_helper(philo, n);
}

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
	if (philo->info->nbr_of_philo % 2 == 0)
		philo->info->time_to_think = philo->info->time_to_eat
			- philo->info->time_to_sleep;
	else if (philo->info->nbr_of_philo % 2 == 1)
		philo->info->time_to_think = ((philo->info->time_to_eat * 2)
				- philo->info->time_to_sleep) * 0.42;
	hypnos(tva() + philo->info->time_to_think);
	eat_sleep_repeat(philo);
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

t_philo	*philo_init(t_info *info, char **argv)
{
	t_philo	*philo;
	int		n;

	philo = malloc(sizeof(t_philo) * info->nbr_of_philo);
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

void	*eat_sleep_repeat(void *philos)
{
	t_philo	*philo;

	philo = (t_philo *)philos;
	if (philo->nbr_of_meals == 0)
		philo->info->nbr_of_full_philo++;
	pthread_mutex_lock(&philo->info->lock);
	if (philo->info->still_alive == 0)
	{
		pthread_mutex_unlock(&philo->info->lock);
		return (NULL);
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
	morpheus(philo);
	return (NULL);
}

void	hypnos(long int time_limit)
{
	while (tva() < time_limit)
		usleep(300);
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

long int	tva(void)
{
	struct timeval	time;
	long int		sacred_time;

	gettimeofday(&time, NULL);
	sacred_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (sacred_time);
}

void	ft_putchar(char c)
{
	write (1, &c, 1);
}

void	ft_putnbr(long int nb)
{
	if (nb < 0)
	{
		ft_putchar('-');
		ft_putnbr(nb * -1);
	}
	else if (nb >= 10)
	{
		ft_putnbr(nb / 10);
		ft_putnbr(nb % 10);
	}
	else
		ft_putchar(nb + '0');
}

//1 - pick up fork;
//2 - is eating;
//3 - is sleeping;
//4 - is thinking;
//5 - has falecido;

void	leave_a_message_after_the_beep(t_philo *philo, int i)
{
	long int	sacred_time;

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
	pthread_mutex_unlock(&philo->info->beep);
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

int	main(int argc, char **argv)
{
	t_info	*info;
	t_philo	*philo;
	int		n;

	n = 0;
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
	if (input_verifications(philo, argc) == 0)
	{
		philo_cleaner(philo);
		return (0);
	}
	while (n < info->nbr_of_philo)
	{
		pthread_create(&philo[n].id, NULL, eat_sleep_repeat, &philo[n]);
		n++;
	}
	are_you_still_there(philo);
	n = 0;
	while (n < info->nbr_of_philo)
	{
		pthread_join(philo[n].id, NULL);
		n++;
	}
	philo_cleaner(philo);
	return (0);
}

/* 1 filosofo loop infinito
segfault com 0 ou nr de filosofos negativo
They're dying randomly
They're not eating properly */