/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joanda-s <joanda-s@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/26 15:34:56 by joanda-s          #+#    #+#             */
/*   Updated: 2024/05/15 16:43:07 by joanda-s         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h>
# include <sys/time.h>
# include <stdio.h>
# include <pthread.h>
# include <stdlib.h>
# include <string.h>
# include <limits.h>

typedef struct s_info
{
	int				nbr_of_philo;
	int				nbr_of_full_philo;
	int				still_alive;
	long int		start_time;
	long int		time_til_death;
	long int		time_to_eat;
	long int		time_to_sleep;
	long int		time_to_think;
	pthread_mutex_t	beep;
	pthread_mutex_t	*fork;
	pthread_mutex_t	lock;
}				t_info;

typedef struct s_philo
{
	int				seat_pos;
	int				still_alive;
	int				nbr_of_meals;
	long int		time_til_death;
	pthread_t		id;
	pthread_mutex_t	chronos;
	t_info			*info;
}				t_philo;

int			input_verifications(t_philo *philo, int argc);
int			main_helper(t_philo *philo, t_info *info, int argc);
long int	tva(void);
long long	ft_llato(const char *str);
t_info		*info_init(char **argv);
t_philo		*philo_init(t_info *info, char **argv);
void		are_you_still_there(t_philo *philo, int n);
void		are_you_still_there_helper(t_philo *philo, int n);
void		athena(t_philo *philo);
void		dionysus(t_philo *philo);
void		dionysus_helper(t_philo *philo);
void		*eat_sleep_repeat(void *philos);
void		ft_putchar(char c);
void		ft_putnbr(long int nb);
void		hypnos(long int time_limit);
void		leave_a_message_after_the_beep(t_philo *philo, int i);
void		leave_a_message_after_the_beep_helper(int i);
void		morpheus(t_philo *philo);
void		philo_cleaner(t_philo *philo);
void		pick_up_forks(t_philo *philo);
void		put_down_forks(t_philo *philo);

#endif